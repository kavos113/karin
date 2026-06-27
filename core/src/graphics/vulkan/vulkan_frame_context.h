#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_FRAME_CONTEXT_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_FRAME_CONTEXT_H

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan_surface.h"

namespace karin
{
class VulkanFrameContext
{
public:
    struct FrameState
    {
        bool needFinish;
        uint8_t frameIndex;

        VkCommandBuffer commandBuffer;
        VkViewport viewport;
        VkRect2D scissor;
        VkImageView targetImageView;
        VkExtent2D targetExtent;
    };

    VulkanFrameContext(std::unique_ptr<IVulkanSurface> surface);
    virtual ~VulkanFrameContext() = default;

    virtual void cleanup();

    FrameState beginFrame();
    void endFrame();
    void resize();

    VkExtent2D extent() const;
    VkFormat surfaceFormat() const;

    void startResizing() const;
    void finishResizing() const;

protected:
    std::unique_ptr<IVulkanSurface> m_surface;

private:
    void createCommandBuffers();
    void createSyncObjects();
    void createViewport();

    uint8_t m_currentFrame = 0;

    std::vector<VkCommandBuffer> m_commandBuffers;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkFence> m_inflightFences;

    VkViewport m_viewport = {};
    VkRect2D m_scissor = {};
    VkExtent2D m_extent = {};

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_FRAME_CONTEXT_H
