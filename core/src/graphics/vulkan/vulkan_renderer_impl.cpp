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
VkRect2D toVkRect(const karin::Rectangle& rect)
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
}

namespace karin
{
VulkanRendererImpl::VulkanRendererImpl(std::unique_ptr<IVulkanSurface> surface)
    : m_surface(std::move(surface))
{
    m_extent = m_surface->extent();
    m_deviceResources = std::make_unique<VulkanDeviceResources>(MAX_FRAMES_IN_FLIGHT);
    m_fontRenderer = std::make_unique<VulkanFontRenderer>(this, MAX_FRAMES_IN_FLIGHT);

    createViewport();
    createCommandBuffers();
    createSyncObjects();

    createVertexBuffer();
    createIndexBuffer();
    createMatrixBuffer();

    createPipeline();
}

void VulkanRendererImpl::cleanUp()
{
    vkDeviceWaitIdle(VulkanContext::instance().device());

    m_fontRenderer->cleanup();

    for (const auto& semaphore : m_imageAvailableSemaphores)
    {
        vkDestroySemaphore(VulkanContext::instance().device(), semaphore, nullptr);
    }
    m_imageAvailableSemaphores.clear();

    for (const auto& semaphore : m_renderFinishedSemaphores)
    {
        vkDestroySemaphore(VulkanContext::instance().device(), semaphore, nullptr);
    }
    m_renderFinishedSemaphores.clear();

    for (const auto& fence : m_inflightFences)
    {
        vkDestroyFence(VulkanContext::instance().device(), fence, nullptr);
    }
    m_inflightFences.clear();

    for (auto& commandBuffer : m_commandBuffers)
    {
        vkFreeCommandBuffers(VulkanContext::instance().device(), VulkanContext::instance().commandPool(), 1, &commandBuffer);
    }
    m_commandBuffers.clear();

    m_vertexBuffer.cleanup();
    m_indexBuffer.cleanup();
    for (size_t i = 0; i < m_projMatrixBuffers.size(); ++i)
    {
        m_projMatrixBuffers[i].cleanup();
    }

    vkDestroyDescriptorSetLayout(VulkanContext::instance().device(), m_projMatrixDescriptorSetLayout, nullptr);

    m_deviceResources->cleanup();

    for (auto &pipeline: m_pipelines | std::views::values)
    {
        pipeline->cleanUp();
    }

    m_surface->cleanUp();
}

bool VulkanRendererImpl::beginDraw()
{
    m_vertexBuffer.mappedData = m_vertexStartPoint;
    m_indexBuffer.mappedData = m_indexStartPoint;
    m_vertexOffset = 0;
    m_indexCount = 0;
    m_drawBatches.clear();

    vkWaitForFences(VulkanContext::instance().device(), 1, &m_inflightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
    if (!m_surface->prepareNextImage(m_imageAvailableSemaphores[m_currentFrame]))
    {
        doResize();
        return false;
    }
    vkResetFences(VulkanContext::instance().device(), 1, &m_inflightFences[m_currentFrame]);

    VkCommandBuffer commandBuffer = m_commandBuffers[m_currentFrame];

    vkResetCommandBuffer(commandBuffer, 0);

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin command buffer");
    }

    m_surface->beforeRender(commandBuffer);

    DrawBatch primaryBatch = {
        .viewport = m_viewport,
        .scissor = m_scissor,
        .renderTargetImageView = m_surface->currentImageView(),
        .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .clearValue = m_clearColor,
        .renderTargetArea = {
            .offset = {0, 0},
            .extent = m_surface->extent(),
        },
        .commands = {},
    };
    m_drawBatches.push_back(primaryBatch);

    return true;
}

void VulkanRendererImpl::endDraw()
{
    m_fontRenderer->flushGlyphUploads();

    VkCommandBuffer commandBuffer = m_commandBuffers[m_currentFrame];

    std::array vertexBuffers = {m_vertexBuffer.buffer};
    std::array<VkDeviceSize, 1> offsets = {};
    vkCmdBindVertexBuffers(
        commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data()
    );
    vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

    for (auto& batch : m_drawBatches)
    {
        if (batch.isOffscreenLayer)
        {
            transitionImageLayout(
                commandBuffer,
                batch.renderTargetImage,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                0,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
            );
        }

        VkRenderingAttachmentInfo colorAttachment = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .imageView = batch.renderTargetImageView,
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

            auto projectionMatrixDescSet = m_projMatrixDescriptorSets[m_currentFrame];
            vkCmdBindDescriptorSets(
                commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                0, 1, &projectionMatrixDescSet,
                0, nullptr
            );
            isBindProjMatrix = true;

            for (const auto& command : m_geometryCommands)
            {
                vkCmdBindDescriptorSets(
                    commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    m_pipelines[PipelineType::Geometry]->pipelineLayout(),
                    1, command.descriptorSets.size(), command.descriptorSets.data(),
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
                auto projectionMatrixDescSet = m_projMatrixDescriptorSets[m_currentFrame];
                vkCmdBindDescriptorSets(
                    commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    m_pipelines[PipelineType::Text]->pipelineLayout(),
                    0, 1, &projectionMatrixDescSet,
                    0, nullptr
                );
            }

            auto glyphAtlasSets = m_fontRenderer->glyphAtlasDescriptorSets();
            vkCmdBindDescriptorSets(
                commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                m_pipelines[PipelineType::Text]->pipelineLayout(),
                2, 1, &glyphAtlasSets[m_currentFrame],
                0, nullptr
            );

            for (const auto& command : m_textCommands)
            {
                vkCmdBindDescriptorSets(
                    commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    m_pipelines[PipelineType::Text]->pipelineLayout(),
                    1, command.descriptorSets.size(), command.descriptorSets.data(),
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
                batch.renderTargetImage,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                VK_ACCESS_SHADER_READ_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
            );

            VkRenderingAttachmentInfo attachmentInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .imageView = m_surface->currentImageView(),
                .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .clearValue = {},
            };
            VkRenderingInfo rInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
                .renderArea = {
                    .offset = {0, 0},
                    .extent = m_surface->extent()
                },
                .layerCount = 1,
                .colorAttachmentCount = 1,
                .pColorAttachments = &attachmentInfo
            };
            vkCmdBeginRendering(commandBuffer, &rInfo);

            vkCmdSetViewport(commandBuffer, 0, 1, &m_viewport);
            vkCmdSetScissor(commandBuffer, 0, 1, &m_scissor);

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

            memcpy(m_vertexBuffer.mappedData, vertices.data(), vertices.size() * sizeof(VulkanPipeline::Vertex));
            m_vertexBuffer.mappedData += vertices.size();
            for (uint16_t& index : indices)
            {
                index += m_vertexOffset;
            }
            memcpy(m_indexBuffer.mappedData, indices.data(), indices.size() * sizeof(uint16_t));
            m_indexBuffer.mappedData += indices.size();

            VertexPushConstants vertData = {
                .model = glm::mat4(1.0f)
            };
            FragPushConstants fragData = {
                .shapeType = static_cast<uint32_t>(ShapeType::Nothing),
                .patternType = static_cast<uint32_t>(PatternType::Image),
                .patternParams = {1.0f, 0.0f, 0.0f, 0.0f}
            };
            VkDescriptorSet descriptorSet = m_deviceResources->offscreenImageDescriptorSet(batch.renderTargetImageView);
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

            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, m_indexCount, 0, 0);
            vkCmdEndRendering(commandBuffer);

            m_indexCount += indices.size();
            m_vertexOffset += static_cast<uint16_t>(vertices.size());
        }
    }

    m_surface->endRender(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer");
    }

    std::array semaphores = {m_imageAvailableSemaphores[m_currentFrame]};
    std::array signalSemaphores = {m_renderFinishedSemaphores[m_currentFrame]};
    std::array<VkPipelineStageFlags, 1> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = static_cast<uint32_t>(semaphores.size()),
        .pWaitSemaphores = semaphores.data(),
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
        .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
        .pSignalSemaphores = signalSemaphores.data(),
    };
    if (vkQueueSubmit(VulkanContext::instance().graphicsQueue(), 1, &submitInfo, m_inflightFences[m_currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    bool ret = m_surface->present(m_renderFinishedSemaphores[m_currentFrame]);
    if (!ret)
    {
        doResize();
    }

    m_deviceResources->clearOffscreenImages();

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanRendererImpl::resize(Size size)
{
    doResize();
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
    memcpy(m_vertexBuffer.mappedData, vertices.data(), vertices.size() * sizeof(VulkanPipeline::Vertex));
    m_vertexBuffer.mappedData += vertices.size();

    for (uint16_t& index : indices)
    {
        index += m_vertexOffset;
    }

    memcpy(m_indexBuffer.mappedData, indices.data(), indices.size() * sizeof(uint16_t));
    m_indexBuffer.mappedData += indices.size();
    m_indexCount += indices.size();

    // TODO: オフセットのオーバーフロー
    m_vertexOffset += static_cast<uint16_t>(vertices.size());

    DrawCommand drawCommand = {
        .indexCount = static_cast<uint32_t>(indices.size()),
        .indexOffset = static_cast<uint32_t>(m_indexCount - indices.size()),
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
                auto descriptorSets = m_deviceResources->gradientPointLutDescriptorSet(p.gradientPoints);
                drawCommand.descriptorSets.push_back(descriptorSets[m_currentFrame]);
            }
            else if constexpr (std::is_same_v<T, RadialGradientPattern>)
            {
                auto descriptorSets = m_deviceResources->gradientPointLutDescriptorSet(p.gradientPoints);
                drawCommand.descriptorSets.push_back(descriptorSets[m_currentFrame]);
            }
            else if constexpr (std::is_same_v<T, ImagePattern>)
            {
                auto descriptorSets = m_deviceResources->textureDescriptorSet(p.image);
                drawCommand.descriptorSets.push_back(descriptorSets[m_currentFrame]);
            }
            else if constexpr (std::is_same_v<T, SolidColorPattern>)
            {
                auto descriptorSets = m_deviceResources->dummyTextureDescriptorSet();
                drawCommand.descriptorSets.push_back(descriptorSets[m_currentFrame]);
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

    // TODO: clear colorを指定できるようにしてもいいかも
    // TODO: alphaを使う
    VulkanImage offscreenImage = m_deviceResources->newOffscreenImage(bounds, m_surface->format());
    DrawBatch batch = {
        .isOffscreenLayer = true,
        .viewport = viewport,
        .scissor = scissor,
        .renderTargetImage = offscreenImage.image,
        .renderTargetImageView = offscreenImage.imageView,
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
}

void VulkanRendererImpl::endOffscreenLayer()
{
    DrawBatch mainBatch = {
        .viewport = m_viewport,
        .scissor = m_scissor,
        .renderTargetImage = VK_NULL_HANDLE,
        .renderTargetImageView = m_surface->currentImageView(),
        .renderTargetImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
        .clearValue = {},
        .renderTargetArea = {
            .offset = {0, 0},
            .extent = m_surface->extent()
        },
        .commands = {},
    };
    m_drawBatches.push_back(mainBatch);
}

void VulkanRendererImpl::createCommandBuffers()
{
    m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = VulkanContext::instance().commandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size()),
    };

    if (vkAllocateCommandBuffers(VulkanContext::instance().device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers");
    }
}

void VulkanRendererImpl::createSyncObjects()
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inflightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(VulkanContext::instance().device(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(VulkanContext::instance().device(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(VulkanContext::instance().device(), &fenceInfo, nullptr, &m_inflightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain sync objects");
        }
    }
}

void VulkanRendererImpl::createVertexBuffer()
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = vertexBufferSize,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (m_vertexBuffer.create(bufferInfo, allocInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer");
    }
    m_vertexStartPoint = m_vertexBuffer.mappedData;
}

void VulkanRendererImpl::createIndexBuffer()
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = indexBufferSize,
        .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (m_indexBuffer.create(bufferInfo, allocInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create index buffer");
    }
    m_indexStartPoint = m_indexBuffer.mappedData;
}

void VulkanRendererImpl::createMatrixBuffer()
{
    VkDescriptorSetLayoutBinding projMatrixLayoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr,
    };
    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &projMatrixLayoutBinding,
    };
    if (vkCreateDescriptorSetLayout(
        VulkanContext::instance().device(), &layoutInfo, nullptr, &m_projMatrixDescriptorSetLayout
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create projection matrix descriptor set layout");
    }

    m_projMatrixData.proj = glm::mat4(1.0f);
    m_projMatrixData.proj[0][0] = 2.0f / static_cast<float>(m_extent.width);
    m_projMatrixData.proj[1][1] = 2.0f / static_cast<float>(m_extent.height);
    m_projMatrixData.proj[3][0] = -1.0f;
    m_projMatrixData.proj[3][1] = -1.0f;

    m_projMatrixDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    m_projMatrixBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    std::vector layouts(MAX_FRAMES_IN_FLIGHT, m_projMatrixDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfoDesc = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = VulkanContext::instance().descriptorPool(),
        .descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
        .pSetLayouts = layouts.data(),
    };
    if (vkAllocateDescriptorSets(
        VulkanContext::instance().device(), &allocInfoDesc, m_projMatrixDescriptorSets.data()
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate projection matrix descriptor sets");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDeviceSize bufferSize = sizeof(MatrixBufferObject);
        VmaAllocationCreateInfo allocInfo = {
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO,
        };

        VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = bufferSize,
            .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        if (m_projMatrixBuffers[i].create(bufferInfo, allocInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create projection matrix buffer");
        }

        memcpy(
            m_projMatrixBuffers[i].mappedData,
            &m_projMatrixData,
            sizeof(MatrixBufferObject)
        );

        VkDescriptorBufferInfo bufferInfoDesc = {
            .buffer = m_projMatrixBuffers[i].buffer,
            .offset = 0,
            .range = sizeof(MatrixBufferObject),
        };
        VkWriteDescriptorSet descriptorWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = m_projMatrixDescriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &bufferInfoDesc,
        };
        vkUpdateDescriptorSets(VulkanContext::instance().device(), 1, &descriptorWrite, 0, nullptr);
    }
}

void VulkanRendererImpl::createPipeline()
{
    std::vector descriptorSetLayouts = {
        m_projMatrixDescriptorSetLayout,
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
        m_surface->format(),
        geometry_vert_spv, geometry_vert_spv_len,
        geometry_frag_spv, geometry_frag_spv_len,
        descriptorSetLayouts, pushConstantRanges
    );

    std::vector textDescriptorSetLayouts = {
        m_projMatrixDescriptorSetLayout,
        m_deviceResources->geometryDescriptorSetLayout(),
        m_fontRenderer->atlasDescriptorSetLayout(),
    };
    m_pipelines[PipelineType::Text] = std::make_unique<VulkanPipeline>(
        m_surface->format(),
        geometry_vert_spv, geometry_vert_spv_len,
        text_frag_spv, text_frag_spv_len,
        textDescriptorSetLayouts, pushConstantRanges
    );
}

void VulkanRendererImpl::createViewport()
{
    m_viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(m_extent.width),
        .height = static_cast<float>(m_extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    m_scissor = {
        .offset = {0, 0},
        .extent = m_extent
    };
}

void VulkanRendererImpl::doResize()
{
    vkDeviceWaitIdle(VulkanContext::instance().device());

    m_surface->resize();
    m_extent = m_surface->extent();

    m_projMatrixData.proj[0][0] = 2.0f / static_cast<float>(m_extent.width);
    m_projMatrixData.proj[1][1] = 2.0f / static_cast<float>(m_extent.height);
    m_projMatrixData.proj[3][0] = -1.0f;
    m_projMatrixData.proj[3][1] = -1.0f;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        memcpy(
            m_projMatrixBuffers[i].mappedData,
            &m_projMatrixData,
            sizeof(MatrixBufferObject)
        );
    }

    createViewport();
}
} // karin