#include "vulkan_renderer_impl.h"

#include <cstring>

#include <algorithm>
#include <iostream>
#include <ranges>

#include <glm/gtc/type_ptr.hpp>

#include "shaders/push_constants.slang"
#include "shaders/shader_code.h"
#include "shaders/shader_layout.h"
#include "vulkan_context.h"
#include "vulkan_helpers.h"

namespace
{
using namespace karin;

VkRect2D toVkRect(const Rectangle& rect)
{
    return VkRect2D{
        .offset = {
            static_cast<int32_t>(rect.pos.x),
            static_cast<int32_t>(rect.pos.y)
        },
        .extent = {
            static_cast<uint32_t>(rect.size.width),
            static_cast<uint32_t>(rect.size.height)
        }
    };
}

Size toKarinSize(const VkExtent2D& extent)
{
    return {
        .width = static_cast<float>(extent.width),
        .height = static_cast<float>(extent.height)
    };
}
}

namespace karin
{
VulkanRendererImpl::VulkanRendererImpl(std::unique_ptr<VulkanFrameContext> frameContext)
    : m_frameContext(std::move(frameContext))
{
    VkExtent2D extent = m_frameContext->extent();

    m_deviceResources = std::make_unique<VulkanDeviceResources>(MAX_FRAMES_IN_FLIGHT);
    m_fontRenderer = std::make_unique<VulkanFontRenderer>(this, MAX_FRAMES_IN_FLIGHT);

    m_geometryBuffer = std::make_unique<VulkanGeometryBuffer>();
    m_viewContext = std::make_unique<VulkanViewContext>(extent.width, extent.height);

    createPipeline();

    m_renderCommandStack.reserve(RENDER_COMMAND_STACK_SIZE);
}

void VulkanRendererImpl::cleanUp()
{
    vkDeviceWaitIdle(VulkanContext::instance().device());

    m_fontRenderer->cleanup();

    m_frameContext->cleanup();
    m_geometryBuffer->cleanup();
    m_viewContext->cleanup();

    m_deviceResources->cleanup();

    for (auto &pipeline: m_pipelines | std::views::values)
    {
        pipeline->cleanUp();
    }
}

bool VulkanRendererImpl::beginDraw()
{
    m_geometryBuffer->reset();
    m_drawBatches.clear();
    m_renderCommandStack.clear();
    m_lastLayerID = 0;
    m_deviceResources->clearOffscreenImages();

    Rectangle targetRect(Point(0, 0), toKarinSize(m_frameContext->extent()));
    RenderState state = {
        .isOffscreenLayer = false,
        .targetRect = targetRect,
        .layerID = m_lastLayerID++
    };
    m_renderCommandStack.push_back(state);

    DrawBatch primaryBatch = {
        .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .clearValue = m_clearColor,
        .commands = {},
    };
    m_drawBatches.push_back(primaryBatch);

    return true;
}

void VulkanRendererImpl::endDraw()
{
    m_fontRenderer->flushGlyphUploads();

    VulkanFrameContext::FrameState state = m_frameContext->beginFrame();
    if (state.needFinish)
    {
        return;
    }

    VkCommandBuffer commandBuffer = state.commandBuffer;
    uint8_t currentFrame = state.frameIndex;

    m_geometryBuffer->bind(commandBuffer);

    for (auto& batch : m_drawBatches)
    {
        if (batch.commands.empty() && batch.loadOp != VK_ATTACHMENT_LOAD_OP_CLEAR  && m_drawBatches.size() > 1)
        {
            continue;
        }

        VkImageView renderTargetImageView;
        // use only offscreen layer
        VkImage renderTargetImage = VK_NULL_HANDLE;

        if (batch.isOffscreenLayer)
        {
            const VulkanTextureResourceDescriptor *texture = m_deviceResources->offscreenImage(batch.layerID, {batch.viewport.width, batch.viewport.height}, m_frameContext->surfaceFormat());
            const VulkanImage *image = texture->image();

            if (batch.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR)
            {
                transitionImageLayout(
                    commandBuffer,
                    image->image,
                    VK_IMAGE_LAYOUT_UNDEFINED,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    0,
                    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                );
            }
            else
            {
                transitionImageLayout(
                    commandBuffer,
                    image->image,
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    VK_ACCESS_SHADER_READ_BIT,
                    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                );
            }

            renderTargetImage = image->image;
            renderTargetImageView = image->imageView;
        }
        else
        {
            batch.viewport = state.viewport;
            batch.scissor = state.scissor;
            batch.targetRect = {
                .offset = {0, 0},
                .extent = state.targetExtent
            };
            renderTargetImageView = state.targetImageView;
        }

        VkRenderingAttachmentInfo colorAttachment = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .imageView = renderTargetImageView,
            .imageLayout = batch.renderTargetImageLayout,
            .loadOp = batch.loadOp,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .clearValue = batch.clearValue
        };
        VkRenderingInfo renderingInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
            .renderArea = {
                .offset = {0, 0},
                .extent = batch.targetRect.extent
            },
            .layerCount = 1,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachment
        };
        vkCmdBeginRendering(commandBuffer, &renderingInfo);

        vkCmdSetViewport(commandBuffer, 0, 1, &batch.viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &batch.scissor);

        PipelineType currentType = PipelineType::Geometry;
        bool isFirstBind = true;
        for (const auto& command : batch.commands)
        {
            if (isFirstBind || currentType != command.pipelineType)
            {
                vkCmdBindPipeline(
                    commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    m_pipelines[command.pipelineType]->pipeline()
                );

                if (command.pipelineType == PipelineType::Text)
                {
                    auto glyphAtlasSets = m_fontRenderer->glyphAtlasDescriptorSets();
                    vkCmdBindDescriptorSets(
                        commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        m_pipelines[PipelineType::Text]->pipelineLayout(),
                        2, 1, &glyphAtlasSets[currentFrame],
                        0, nullptr
                    );
                }
            }
            if (isFirstBind)
            {
                m_viewContext->bind(
                    commandBuffer,
                    m_pipelines[command.pipelineType]->pipelineLayout(),
                    batch.layerID,
                    currentFrame
                );

                isFirstBind = false;
            }

            if (command.pipelineType != PipelineType::Shadow)
            {
                auto descriptorSets = command.textureResources
                    | std::views::transform(
                        [currentFrame](const VulkanTextureResourceDescriptor *resource)
                        {
                            return resource->descriptorSet(currentFrame);
                        })
                    | std::ranges::to<std::vector>();

                vkCmdBindDescriptorSets(
                    commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    m_pipelines[command.pipelineType]->pipelineLayout(),
                    1, descriptorSets.size(), descriptorSets.data(),
                    0, nullptr
                );
            }

            PushConstant push = {command.vertData, command.fragData};
            vkCmdPushConstants(
                commandBuffer,
                m_pipelines[command.pipelineType]->pipelineLayout(),
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0, sizeof(PushConstant), &push
            );

            VkRect2D scissor = command.scissor.value_or(batch.scissor);
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            vkCmdDrawIndexed(commandBuffer, command.indexCount, 1, command.indexOffset, 0, 0);
        }

        vkCmdEndRendering(commandBuffer);

        if (batch.isOffscreenLayer)
        {
            transitionImageLayout(
                commandBuffer,
                renderTargetImage,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                VK_ACCESS_SHADER_READ_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
            );
        }
    }

    m_frameContext->endFrame();
}

void VulkanRendererImpl::resize(Size size)
{
    m_frameContext->resize();

    VkExtent2D extent = m_frameContext->extent();
    m_viewContext->resize(extent.width, extent.height);
}

void VulkanRendererImpl::addCommand(
    const std::vector<VulkanPipeline::Vertex>& vertices,
    std::vector<uint16_t>& indices,
    const FragPushConstants& fragData,
    const VertexPushConstants& vertData,
    const Pattern& pattern,
    PipelineType pipelineType,
    std::optional<Rectangle> clipRect
)
{
    uint32_t indexOffset = m_geometryBuffer->append(vertices, indices);

    DrawCommand drawCommand = {
        .indexCount = static_cast<uint32_t>(indices.size()),
        .indexOffset = indexOffset,
        .fragData = fragData,
        .vertData = vertData,
        .pipelineType = pipelineType,
        .scissor = clipRect.has_value() ? std::make_optional(toVkRect(clipRect.value())) : std::nullopt,
    };

    if (pipelineType == PipelineType::Shadow)
    {
        m_drawBatches.back().commands.push_back(drawCommand);
        return;
    }

    std::visit(
        [this, &drawCommand]<typename T0>(const T0& p)
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, LinearGradientPattern>)
            {
                auto texture = m_deviceResources->gradientPointLut(p.gradientPoints);
                drawCommand.textureResources.push_back(texture);
            }
            else if constexpr (std::is_same_v<T, RadialGradientPattern>)
            {
                auto texture = m_deviceResources->gradientPointLut(p.gradientPoints);
                drawCommand.textureResources.push_back(texture);
            }
            else if constexpr (std::is_same_v<T, ImagePattern>)
            {
                auto texture = m_deviceResources->texture(p.image);
                drawCommand.textureResources.push_back(texture);
            }
            else if constexpr (std::is_same_v<T, SolidColorPattern>)
            {
                auto texture = m_deviceResources->dummyTexture();
                drawCommand.textureResources.push_back(texture);
            }
            else
            {
                throw std::runtime_error("Unsupported pattern type");
            }
        }, pattern
    );

    m_drawBatches.back().commands.push_back(drawCommand);
}

void VulkanRendererImpl::beginOffscreenLayer(const Rectangle& bounds, float alpha)
{
    if (m_lastLayerID >= MAX_OFFSCREEN_LAYER_SIZE)
    {
        throw std::invalid_argument("too many offscreen layer");
    }

    VkViewport viewport = {
        .x = 0,
        .y = 0,
        .width = bounds.size.width,
        .height = bounds.size.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };
    VkRect2D scissor = {
        .offset = {
            0, 0
        },
        .extent = {
            static_cast<uint32_t>(bounds.size.width),
            static_cast<uint32_t>(bounds.size.height)
        }
    };

    RenderState state = {
        .isOffscreenLayer = true,
        .targetRect = bounds,
        .layerID = m_lastLayerID,
        .alpha = alpha,
    };
    m_renderCommandStack.push_back(state);

    // TODO: clear colorを指定できるようにしてもいいかも
    DrawBatch batch = {
        .isOffscreenLayer = true,
        .viewport = viewport,
        .scissor = scissor,
        .alpha = alpha,
        .layerID = m_lastLayerID,
        .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .clearValue = {
            .color = {{0.0f, 0.0f, 0.0f, 0.0f}}
        },
        .targetRect = {
            .offset = {
                static_cast<int32_t>(bounds.pos.x),
                static_cast<int32_t>(bounds.pos.y),
            },
            .extent = {
                static_cast<uint32_t>(bounds.size.width),
                static_cast<uint32_t>(bounds.size.height)
            }
        },
        .commands = {},
    };

    m_drawBatches.push_back(batch);

    m_viewContext->setProjMatrix(bounds, m_lastLayerID);

    m_lastLayerID++;
}

void VulkanRendererImpl::endOffscreenLayer()
{
    RenderState currentState = m_renderCommandStack.back();

    auto texture = m_deviceResources->offscreenImage(currentState.layerID, currentState.targetRect.size, m_frameContext->surfaceFormat());
    Rectangle uv = m_deviceResources->offscreenImageUv(currentState.layerID, currentState.targetRect.size);
    std::vector<VulkanPipeline::Vertex> vertices = {
        {
            .pos = {currentState.targetRect.pos.x, currentState.targetRect.pos.y},
            .uv = {uv.pos.x, uv.pos.y},
        },
        {
            .pos = {currentState.targetRect.pos.x + currentState.targetRect.size.width, currentState.targetRect.pos.y},
            .uv = {uv.pos.x + uv.size.width, uv.pos.y},
        },
        {
            .pos = {currentState.targetRect.pos.x + currentState.targetRect.size.width, currentState.targetRect.pos.y + currentState.targetRect.size.height},
            .uv = {uv.pos.x + uv.size.width, uv.pos.y + uv.size.height},
        },
        {
            .pos = {currentState.targetRect.pos.x, currentState.targetRect.pos.y + currentState.targetRect.size.height},
            .uv = {uv.pos.x, uv.pos.y + uv.size.height},
        }
    };
    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    uint32_t indexOffset = m_geometryBuffer->append(vertices, indices);

    VertexPushConstants vertData = {
        .model = glm::mat4(1.0f)
    };
    FragPushConstants fragData = {
        .shapeType = static_cast<uint32_t>(ShapeType::Nothing),
        .patternType = static_cast<uint32_t>(PatternType::Image),
        .patternParams = {currentState.alpha, 0.0f, 0.0f, 0.0f}
    };

    m_renderCommandStack.pop_back();
    RenderState state = m_renderCommandStack.back();

    DrawBatch batch;
    if (state.layerID == 0) // target is main window
    {
        batch = DrawBatch{
            .isOffscreenLayer = false,
            .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
            .clearValue = {},
            .commands = {
                DrawCommand{
                    .indexCount = static_cast<uint32_t>(indices.size()),
                    .indexOffset = indexOffset,
                    .fragData = fragData,
                    .vertData = vertData,
                    .pipelineType = PipelineType::Geometry,
                    .scissor = std::nullopt,
                    .textureResources = {texture}
                }
            },
        };
    }
    else // target is also offscreen layer
    {
        VkViewport viewport = {
            .x = 0,
            .y = 0,
            .width = state.targetRect.size.width,
            .height = state.targetRect.size.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };
        VkRect2D scissor = {
            .offset = {0, 0},
            .extent = {
                static_cast<uint32_t>(state.targetRect.size.width),
                static_cast<uint32_t>(state.targetRect.size.height)
            }
        };

        batch = DrawBatch{
            .isOffscreenLayer = true,
            .viewport = viewport,
            .scissor = scissor,
            .alpha = state.alpha,
            .layerID = state.layerID,
            .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
            .clearValue = {},
            .targetRect = {
                .offset = {
                    static_cast<int32_t>(state.targetRect.pos.x),
                    static_cast<int32_t>(state.targetRect.pos.y),
                },
                .extent = {
                    static_cast<uint32_t>(state.targetRect.size.width),
                    static_cast<uint32_t>(state.targetRect.size.height)
                }
            },
            .commands = {
                DrawCommand{
                    .indexCount = static_cast<uint32_t>(indices.size()),
                    .indexOffset = indexOffset,
                    .fragData = fragData,
                    .vertData = vertData,
                    .pipelineType = PipelineType::Geometry,
                    .scissor = std::nullopt,
                    .textureResources = {texture}
                }
            },
        };
    }

    m_drawBatches.push_back(batch);
}

void VulkanRendererImpl::createPipeline()
{
    std::vector pushConstantRanges = {
        VkPushConstantRange{
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            .offset = 0,
            .size = sizeof(PushConstant)
        },
    };

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts(gen::geometry_frag_main::max_set + 1);
    descriptorSetLayouts[gen::geometry_frag_main::matrices_set] = m_viewContext->descriptorSetLayout();
    descriptorSetLayouts[gen::geometry_frag_main::tex_tex_set] = m_deviceResources->geometryDescriptorSetLayout();
    m_pipelines[PipelineType::Geometry] = std::make_unique<VulkanPipeline>(
        m_frameContext->surfaceFormat(),
        gen::vertex_vert_main_code.data(), gen::vertex_vert_main_code.size(),
        gen::geometry_frag_main_code.data(), gen::geometry_frag_main_code.size(),
        descriptorSetLayouts, pushConstantRanges
    );

    std::vector<VkDescriptorSetLayout> textDescriptorSetLayouts(gen::text_frag_main::max_set + 1);
    textDescriptorSetLayouts[gen::text_frag_main::matrices_set] = m_viewContext->descriptorSetLayout();
    textDescriptorSetLayouts[gen::text_frag_main::tex_tex_set] = m_deviceResources->geometryDescriptorSetLayout();
    textDescriptorSetLayouts[gen::text_frag_main::glyphAtlas_tex_set] = m_fontRenderer->atlasDescriptorSetLayout();
    m_pipelines[PipelineType::Text] = std::make_unique<VulkanPipeline>(
        m_frameContext->surfaceFormat(),
        gen::vertex_vert_main_code.data(), gen::vertex_vert_main_code.size(),
        gen::text_frag_main_code.data(), gen::text_frag_main_code.size(),
        textDescriptorSetLayouts, pushConstantRanges
    );

    std::vector<VkDescriptorSetLayout> shadowDescriptorSetLayouts(gen::shadow_frag_main::max_set + 1);
    shadowDescriptorSetLayouts[gen::shadow_frag_main::matrices_set] = m_viewContext->descriptorSetLayout();
    m_pipelines[PipelineType::Shadow] = std::make_unique<VulkanPipeline>(
        m_frameContext->surfaceFormat(),
        gen::vertex_vert_main_code.data(), gen::vertex_vert_main_code.size(),
        gen::shadow_frag_main_code.data(), gen::shadow_frag_main_code.size(),
        descriptorSetLayouts, pushConstantRanges
    );
}
} // karin