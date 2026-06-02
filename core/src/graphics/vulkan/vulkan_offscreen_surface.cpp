#include "vulkan_offscreen_surface.h"

#include <stdexcept>

#include "vulkan_context.h"

namespace karin
{
VulkanOffscreenSurface::VulkanOffscreenSurface(Size size)
    : m_size(size)
{
    createBuffers(static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height));
}

void VulkanOffscreenSurface::createFrameBuffers(VkRenderPass renderPass)
{
    VkFramebufferCreateInfo framebufferInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = renderPass,
        .attachmentCount = 1,
        .pAttachments = &m_image.imageView,
        .width = static_cast<uint32_t>(m_size.width),
        .height = static_cast<uint32_t>(m_size.height),
        .layers = 1
    };
    if (vkCreateFramebuffer(VulkanContext::instance().device(), &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create framebuffer for offscreen surface");
    }
}

void VulkanOffscreenSurface::destroyFrameBuffers()
{
    if (m_framebuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(VulkanContext::instance().device(), m_framebuffer, nullptr);
        m_framebuffer = VK_NULL_HANDLE;
    }
}

void VulkanOffscreenSurface::cleanUp()
{
    m_image.cleanup();
    m_stagingBuffer.cleanup();
}

void VulkanOffscreenSurface::resize(VkRenderPass renderPass)
{
    destroyFrameBuffers();
    createBuffers(static_cast<uint32_t>(m_size.width), static_cast<uint32_t>(m_size.height));
    createFrameBuffers(renderPass);
}

bool VulkanOffscreenSurface::prepareNextImage(VkSemaphore semaphore)
{
    return true;
}

bool VulkanOffscreenSurface::present(VkSemaphore waitSemaphore) const
{
    return true;
}

VkExtent2D VulkanOffscreenSurface::extent() const
{
    return {
        static_cast<uint32_t>(m_size.width),
        static_cast<uint32_t>(m_size.height),
    };
}

VkFormat VulkanOffscreenSurface::format() const
{
    return VK_FORMAT_B8G8R8A8_UNORM;
}

VkFramebuffer VulkanOffscreenSurface::currentFrameBuffer() const
{
    return m_framebuffer;
}

std::vector<std::byte> VulkanOffscreenSurface::getImageData() const
{
}

void VulkanOffscreenSurface::createBuffers(uint32_t width, uint32_t height)
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
    };
    VkImageCreateInfo imageInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_B8G8R8A8_UNORM,
        .extent = {
            .width = width,
            .height = height,
            .depth = 1,
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = nullptr,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = imageInfo.format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    if (m_image.create(imageInfo, allocInfo, viewInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create offscreen surface image");
    }

    VmaAllocationCreateInfo stagingBufferAllocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    VkDeviceSize stagingBufferSize = width * height * 4;
    VkBufferCreateInfo stagingBufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = stagingBufferSize,
        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    if (m_stagingBuffer.create(stagingBufferInfo, stagingBufferAllocInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create staging buffer for offscreen surface");
    }
}
} // karin