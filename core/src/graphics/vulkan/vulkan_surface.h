#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_SURFACE_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_SURFACE_H

#include <cstdint>

#include <vulkan/vulkan.h>

namespace karin
{
class IVulkanSurface
{
public:
    virtual ~IVulkanSurface() = default;

    virtual void cleanUp() = 0;
    virtual void resize(VkRenderPass renderPass) = 0;

    virtual bool prepareNextImage(VkSemaphore semaphore) = 0;
    virtual void setViewPorts(VkCommandBuffer commandBuffer) const = 0;

    virtual bool present(VkSemaphore waitSemaphore) const = 0;

    virtual VkExtent2D extent() const = 0;
    virtual VkFormat format() const = 0;
    virtual uint32_t imageCount() const = 0;
    virtual VkFramebuffer currentFrameBuffer() const = 0;

    virtual void startResizing() = 0;
    virtual void finishResizing() = 0;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_SURFACE_H
