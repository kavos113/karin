#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_SURFACE_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_SURFACE_H

#include <cstddef>
#include <cstdint>

#include <vector>

#include <vulkan/vulkan.h>

#include <karin/common/geometry/size.h>
#include "vulkan_surface.h"
#include "vulkan_buffer.h"

namespace karin
{
class VulkanOffscreenSurface : public IVulkanSurface
{
public:
    VulkanOffscreenSurface(Size size);
    ~VulkanOffscreenSurface() override = default;

    void cleanUp() override;
    void resize() override;

    bool prepareNextImage(VkSemaphore semaphore) override;
    void beforeRender(VkCommandBuffer commandBuffer) override;
    void endRender(VkCommandBuffer commandBuffer) override;
    bool present(VkSemaphore waitSemaphore) const override;

    VkExtent2D extent() const override;
    VkFormat format() const override;
    VkImageView currentImageView() const override;

    std::vector<std::byte> getImageData() const;

private:
    void createBuffers(uint32_t width, uint32_t height);

    uint32_t m_width;
    uint32_t m_height;

    VulkanImage m_image;
    VulkanBuffer<std::byte> m_stagingBuffer;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_SURFACE_H
