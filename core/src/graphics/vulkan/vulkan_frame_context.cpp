#include "vulkan_frame_context.h"

#include <stdexcept>
#include <array>

#include "vulkan_context.h"

namespace karin
{
VulkanFrameContext::VulkanFrameContext(std::unique_ptr<IVulkanSurface> surface)
    : m_surface(std::move(surface))
{
    m_extent = m_surface->extent();

    createCommandBuffers();
    createSyncObjects();
    createViewport();
}

void VulkanFrameContext::cleanup()
{
    for (const auto& semaphore : m_imageAvailableSemaphores)
    {
        vkDestroySemaphore(VulkanContext::instance().device(), semaphore, nullptr);
    }
    m_imageAvailableSemaphores.clear();

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

    m_surface->cleanup();
}

VulkanFrameContext::FrameState VulkanFrameContext::beginFrame()
{
    vkWaitForFences(VulkanContext::instance().device(), 1, &m_inflightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
    if (!m_surface->prepareNextImage(m_imageAvailableSemaphores[m_currentFrame]))
    {
        resize();
        return {
            .needFinish = true
        };
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

    return {
        .needFinish = false,
        .frameIndex = m_currentFrame,
        .commandBuffer = commandBuffer,
        .viewport = m_viewport,
        .scissor = m_scissor,
        .targetImageView = m_surface->currentImageView(),
        .targetExtent = m_extent
    };
}

void VulkanFrameContext::endFrame()
{
    VkCommandBuffer commandBuffer = m_commandBuffers[m_currentFrame];

    m_surface->endRender(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer");
    }

    std::array semaphores = {m_imageAvailableSemaphores[m_currentFrame]};
    std::vector<VkSemaphore> signalSemaphores = m_surface->renderFinishSemaphore();
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

    bool ret = m_surface->present();
    if (!ret)
    {
        resize();
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanFrameContext::resize()
{
    vkDeviceWaitIdle(VulkanContext::instance().device());

    m_surface->resize();
    m_extent = m_surface->extent();

    createViewport();
}

VkExtent2D VulkanFrameContext::extent() const
{
    return m_extent;
}

VkFormat VulkanFrameContext::surfaceFormat() const
{
    return m_surface->format();
}

void VulkanFrameContext::startResizing() const
{
    m_surface->startResizing();
}

void VulkanFrameContext::finishResizing() const
{
    m_surface->finishResizing();
}

void VulkanFrameContext::createCommandBuffers()
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

void VulkanFrameContext::createSyncObjects()
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
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
            vkCreateFence(VulkanContext::instance().device(), &fenceInfo, nullptr, &m_inflightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain sync objects");
        }
    }
}

void VulkanFrameContext::createViewport()
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
} // karin