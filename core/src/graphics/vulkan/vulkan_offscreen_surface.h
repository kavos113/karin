#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_SURFACE_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_SURFACE_H

#include <vector>

#include <vulkan/vulkan.h>
#include "vma.h"

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

    void createFrameBuffers(VkRenderPass renderPass) override;
    void destroyFrameBuffers() override;
    void cleanUp() override;
    void resize(VkRenderPass renderPass) override;

    bool prepareNextImage(VkSemaphore semaphore) override;
    bool present(VkSemaphore waitSemaphore) const override;

    VkExtent2D extent() const override;
    VkFormat format() const override;
    VkFramebuffer currentFrameBuffer() const override;

    std::vector<std::byte> getImageData() const;

private:
    void createBuffers(uint32_t width, uint32_t height);

    Size m_size;

    VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
    VulkanImage m_image;
    VulkanBuffer<std::byte> m_stagingBuffer;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_SURFACE_H
