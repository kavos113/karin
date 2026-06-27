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
    };

    VulkanFrameContext();
    ~VulkanFrameContext() = default;

    void cleanup();

    FrameState beginFrame();
    void endFrame();

private:
    void createCommandBuffers();
    void createSyncObjects();

    std::unique_ptr<IVulkanSurface> m_surface;

    uint8_t m_currentFrame = 0;

    std::vector<VkCommandBuffer> m_commandBuffers;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkFence> m_inflightFences;

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_FRAME_CONTEXT_H
