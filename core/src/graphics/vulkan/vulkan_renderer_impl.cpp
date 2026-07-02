#include "vulkan_renderer_impl.h"

#include <cstring>

#include <algorithm>
#include <iostream>
#include <ranges>

#include <glm/gtc/type_ptr.hpp>

#include "shaders/push_constants.h"
#include "shaders/shaders.h"
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

    VkCommandBuffer commandBuffer = state.commandBuffer;
    uint8_t currentFrame = state.frameIndex;

    m_geometryBuffer->bind(commandBuffer);

    for (auto& batch : m_drawBatches)
    {
        VkImageView renderTargetImageView;
        // use only offscreen layer
        VkImage renderTargetImage = VK_NULL_HANDLE;

        if (batch.isOffscreenLayer)
        {
            VulkanImage *image = m_deviceResources->offscreenImage(batch.layerID);

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

            renderTargetImage = image->image;
            renderTargetImageView = image->imageView;
        }
        else
        {
            batch.viewport = state.viewport;
            batch.scissor = state.scissor;
            batch.renderTargetArea = {
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
            .renderArea = batch.renderTargetArea,
            .layerCount = 1,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachment
        };
        vkCmdBeginRendering(commandBuffer, &renderingInfo);

        vkCmdSetViewport(commandBuffer, 0, 1, &batch.viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &batch.scissor);

        std::vector<DrawCommand> m_geometryCommands;
        std::vector<DrawCommand> m_textCommands;
        for (const auto& command : batch.commands)
        {
            switch (command.pipelineType)
            {
            case PipelineType::Geometry:
                m_geometryCommands.push_back(command);
                break;
            case PipelineType::Text:
                m_textCommands.push_back(command);
                break;
            }
        }

        bool isBindProjMatrix = false;

        if (!m_geometryCommands.empty())
        {
            vkCmdBindPipeline(
                commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                m_pipelines[PipelineType::Geometry]->pipeline()
            );

            m_viewContext->bind(
                commandBuffer,
                m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                batch.layerID,
                currentFrame
            );
            isBindProjMatrix = true;

            for (const auto& command : m_geometryCommands)
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
                    m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                    1, descriptorSets.size(), descriptorSets.data(),
                    0, nullptr
                );

                vkCmdPushConstants(
                    commandBuffer,
                    m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                    VK_SHADER_STAGE_FRAGMENT_BIT,
                    0, sizeof(FragPushConstants), &command.fragData
                );
                vkCmdPushConstants(
                    commandBuffer,
                    m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                    VK_SHADER_STAGE_VERTEX_BIT,
                    sizeof(FragPushConstants), sizeof(VertexPushConstants), &command.vertData
                );

                if (command.scissor.has_value())
                {
                    vkCmdSetScissor(commandBuffer, 0, 1, &command.scissor.value());
                }

                vkCmdDrawIndexed(commandBuffer, command.indexCount, 1, command.indexOffset, 0, 0);
            }
        }

        if (!m_textCommands.empty())
        {
            vkCmdBindPipeline(
                commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                m_pipelines[PipelineType::Text]->pipeline()
            );

            if (!isBindProjMatrix)
            {
                m_viewContext->bind(
                    commandBuffer,
                    m_pipelines[PipelineType::Text]->pipelineLayout(),
                    batch.layerID,
                    currentFrame
                );
                isBindProjMatrix = true;
            }

            auto glyphAtlasSets = m_fontRenderer->glyphAtlasDescriptorSets();
            vkCmdBindDescriptorSets(
                commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                m_pipelines[PipelineType::Text]->pipelineLayout(),
                2, 1, &glyphAtlasSets[currentFrame],
                0, nullptr
            );

            for (const auto& command : m_textCommands)
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
                    m_pipelines[PipelineType::Text]->pipelineLayout(),
                    1, descriptorSets.size(), descriptorSets.data(),
                    0, nullptr
                );

                vkCmdPushConstants(
                    commandBuffer,
                    m_pipelines[PipelineType::Text]->pipelineLayout(),
                    VK_SHADER_STAGE_FRAGMENT_BIT,
                    0, sizeof(FragPushConstants), &command.fragData
                );
                vkCmdPushConstants(
                    commandBuffer,
                    m_pipelines[PipelineType::Text]->pipelineLayout(),
                    VK_SHADER_STAGE_VERTEX_BIT,
                    sizeof(FragPushConstants), sizeof(VertexPushConstants), &command.vertData
                );

                if (command.scissor.has_value())
                {
                    vkCmdSetScissor(commandBuffer, 0, 1, &command.scissor.value());
                }

                vkCmdDrawIndexed(commandBuffer, command.indexCount, 1, command.indexOffset, 0, 0);
            }
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

            VkRenderingAttachmentInfo attachmentInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .imageView = state.targetImageView,
                .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .clearValue = {},
            };
            VkRenderingInfo rInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
                .renderArea = {
                    .offset = {0, 0},
                    .extent = state.targetExtent
                },
                .layerCount = 1,
                .colorAttachmentCount = 1,
                .pColorAttachments = &attachmentInfo
            };
            vkCmdBeginRendering(commandBuffer, &rInfo);

            vkCmdSetViewport(commandBuffer, 0, 1, &state.viewport);
            vkCmdSetScissor(commandBuffer, 0, 1, &state.scissor);

            vkCmdBindPipeline(
                commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                m_pipelines[PipelineType::Geometry]->pipeline()
            );

            std::vector<VulkanPipeline::Vertex> vertices = {
                {
                    .pos = {batch.viewport.x, batch.viewport.y},
                    .uv = {0.0f, 0.0f},
                },
                {
                    .pos = {batch.viewport.x + batch.viewport.width, batch.viewport.y},
                    .uv = {1.0f, 0.0f},
                },
                {
                    .pos = {batch.viewport.x + batch.viewport.width, batch.viewport.y + batch.viewport.height},
                    .uv = {1.0f, 1.0f},
                },
                {
                    .pos = {batch.viewport.x, batch.viewport.y + batch.viewport.height},
                    .uv = {0.0f, 1.0f},
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
                .patternParams = {1.0f, 0.0f, 0.0f, 0.0f}
            };
            VkDescriptorSet descriptorSet = m_deviceResources->offscreenImageDescriptorSet(renderTargetImageView);
            vkCmdBindDescriptorSets(
                commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                1, 1, &descriptorSet,
                0, nullptr
            );

            vkCmdPushConstants(
                commandBuffer,
                m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                VK_SHADER_STAGE_FRAGMENT_BIT,
                0, sizeof(FragPushConstants), &fragData
            );
            vkCmdPushConstants(
                commandBuffer,
                m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                VK_SHADER_STAGE_VERTEX_BIT,
                sizeof(FragPushConstants), sizeof(VertexPushConstants), &vertData
            );

            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, indexOffset, 0, 0);
            vkCmdEndRendering(commandBuffer);
        }
    }

    m_frameContext->endFrame();

    m_deviceResources->clearOffscreenImages();
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
    VkViewport viewport = {
        .x = bounds.pos.x,
        .y = bounds.pos.y,
        .width = bounds.size.width,
        .height = bounds.size.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };
    VkRect2D scissor = {
        .offset = {
            static_cast<int32_t>(bounds.pos.x),
            static_cast<int32_t>(bounds.pos.y)
        },
        .extent = {
            static_cast<uint32_t>(bounds.size.width),
            static_cast<uint32_t>(bounds.size.height)
        }
    };

    RenderState state = {
        .isOffscreenLayer = true,
        .targetRect = bounds,
        .layerID = m_lastLayerID
    };
    m_renderCommandStack.push_back(state);

    // TODO: clear colorを指定できるようにしてもいいかも
    // TODO: alphaを使う
    m_deviceResources->newOffscreenImage(bounds, m_frameContext->surfaceFormat(), m_lastLayerID);
    DrawBatch batch = {
        .isOffscreenLayer = true,
        .viewport = viewport,
        .scissor = scissor,
        .layerID = m_lastLayerID,
        .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .clearValue = {
            .color = {{0.0f, 0.0f, 0.0f, 0.0f}}
        },
        .renderTargetArea = {
            .offset = {0, 0},
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
    RenderState state = m_renderCommandStack.back();
    m_renderCommandStack.pop_back();

    DrawBatch batch;
    if (state.layerID == 0)
    {
        batch = DrawBatch{
            .isOffscreenLayer = false,
            .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .clearValue = m_clearColor,
            .commands = {},
        };
    }
    else
    {
        VkViewport viewport = {
            .x = state.targetRect.pos.x,
            .y = state.targetRect.pos.y,
            .width = state.targetRect.size.width,
            .height = state.targetRect.size.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };
        VkRect2D scissor = {
            .offset = {
                static_cast<int32_t>(state.targetRect.pos.x),
                static_cast<int32_t>(state.targetRect.pos.y)
            },
            .extent = {
                static_cast<uint32_t>(state.targetRect.size.width),
                static_cast<uint32_t>(state.targetRect.size.height)
            }
        };

        batch = DrawBatch{
            .isOffscreenLayer = true,
            .viewport = viewport,
            .scissor = scissor,
            .layerID = state.layerID,
            .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .clearValue = {
                .color = {{0.0f, 0.0f, 0.0f, 0.0f}}
            },
            .renderTargetArea = {
                .offset = {0, 0},
                .extent = {
                    static_cast<uint32_t>(state.targetRect.size.width),
                    static_cast<uint32_t>(state.targetRect.size.height)
                }
            },
            .commands = {},
        };
    }

    m_drawBatches.push_back(batch);
}

void VulkanRendererImpl::createPipeline()
{
    std::vector descriptorSetLayouts = {
        m_viewContext->descriptorSetLayout(),
        m_deviceResources->geometryDescriptorSetLayout(),
    };
    std::vector pushConstantRanges = {
        VkPushConstantRange{
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .offset = 0,
            .size = sizeof(FragPushConstants)
        },
        VkPushConstantRange{
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .offset = sizeof(FragPushConstants),
            .size = sizeof(VertexPushConstants)
        }
    };
    m_pipelines[PipelineType::Geometry] = std::make_unique<VulkanPipeline>(
        m_frameContext->surfaceFormat(),
        geometry_vert_spv, geometry_vert_spv_len,
        geometry_frag_spv, geometry_frag_spv_len,
        descriptorSetLayouts, pushConstantRanges
    );

    std::vector textDescriptorSetLayouts = {
        m_viewContext->descriptorSetLayout(),
        m_deviceResources->geometryDescriptorSetLayout(),
        m_fontRenderer->atlasDescriptorSetLayout(),
    };
    m_pipelines[PipelineType::Text] = std::make_unique<VulkanPipeline>(
        m_frameContext->surfaceFormat(),
        geometry_vert_spv, geometry_vert_spv_len,
        text_frag_spv, text_frag_spv_len,
        textDescriptorSetLayouts, pushConstantRanges
    );
}
} // karin