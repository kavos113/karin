#ifndef GRAPHICS_VULKAN_VULKAN_SURFACE_H
#define GRAPHICS_VULKAN_VULKAN_SURFACE_H

#include <vulkan/vulkan.h>

namespace karin
{
class IVulkanSurface
{
public:
    virtual ~IVulkanSurface() = default;

    virtual void cleanup() = 0;
    virtual void resize() = 0;

    /**
     * @param semaphore when complete to prepare image, signal this semaphore.
     * @return if need resize
     */
    virtual bool prepareNextImage(VkSemaphore semaphore) = 0;
    virtual void beforeRender(VkCommandBuffer commandBuffer) = 0;
    virtual void endRender(VkCommandBuffer commandBuffer) = 0;

    virtual bool present() const = 0;
    /**
     * @return semaphores to wait finish rendering. if don't need wait, length = 0
     */
    virtual std::vector<VkSemaphore> renderFinishSemaphore() const = 0;

    virtual VkExtent2D extent() const = 0;
    virtual VkFormat format() const = 0;
    virtual VkImageView currentImageView() const = 0;

    virtual void startResizing() {}
    virtual void finishResizing() {}
};
} // karin

#endif //GRAPHICS_VULKAN_VULKAN_SURFACE_H
