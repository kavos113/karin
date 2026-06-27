#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_SURFACE_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_SURFACE_H

#include <vulkan/vulkan.h>

namespace karin
{
class IVulkanSurface
{
public:
    virtual ~IVulkanSurface() = default;

    virtual void cleanup() = 0;
    virtual void resize() = 0;

    virtual bool prepareNextImage(VkSemaphore semaphore) = 0;
    virtual void beforeRender(VkCommandBuffer commandBuffer) = 0;
    virtual void endRender(VkCommandBuffer commandBuffer) = 0;
    virtual bool present(VkSemaphore waitSemaphore) const = 0;

    virtual VkExtent2D extent() const = 0;
    virtual VkFormat format() const = 0;
    virtual VkImageView currentImageView() const = 0;

    virtual void startResizing() {}
    virtual void finishResizing() {}
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_SURFACE_H
