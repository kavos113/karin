#include "vulkan_offscreen_surface.h"

#include <cstring>

#include <stdexcept>

#include "vulkan_context.h"
#include "vulkan_helpers.h"

namespace karin
{
VulkanOffscreenSurface::VulkanOffscreenSurface(Size size)
    : m_width(static_cast<uint32_t>(size.width)),
      m_height(static_cast<uint32_t>(size.height))
{
    VulkanContext::instance().initOffscreenDevices();

    createBuffers(static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height));
}

void VulkanOffscreenSurface::cleanUp()
{
    m_image.cleanup();
    m_stagingBuffer.cleanup();
}

void VulkanOffscreenSurface::resize()
{
    cleanUp();

    createBuffers(m_width, m_height);
}

bool VulkanOffscreenSurface::prepareNextImage(VkSemaphore semaphore)
{
    // just for send signal
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 0,
        .pCommandBuffers = nullptr,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &semaphore,
    };
    if (vkQueueSubmit(VulkanContext::instance().graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit offscreen surface image preparation command buffer");
    }

    return true;
}

void VulkanOffscreenSurface::beforeRender(VkCommandBuffer commandBuffer)
{
    transitionImageLayout(
        commandBuffer,
        m_image.image,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        0,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    );
}

void VulkanOffscreenSurface::endRender(VkCommandBuffer commandBuffer)
{
}

bool VulkanOffscreenSurface::present(VkSemaphore waitSemaphore) const
{
    // just for send signal
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &waitSemaphore,
        .pWaitDstStageMask = &waitStage,
        .commandBufferCount = 0,
        .pCommandBuffers = nullptr,
    };
    if (vkQueueSubmit(VulkanContext::instance().graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit offscreen surface presentation command buffer");
    }

    return true;
}

VkExtent2D VulkanOffscreenSurface::extent() const
{
    return {m_width, m_height};
}

VkFormat VulkanOffscreenSurface::format() const
{
    return VK_FORMAT_B8G8R8A8_UNORM;
}

VkImageView VulkanOffscreenSurface::currentImageView() const
{
    return m_image.imageView;
}

std::vector<std::byte> VulkanOffscreenSurface::getImageData() const
{
    VkCommandBuffer commandBuffer = VulkanContext::instance().beginSingleTimeCommands();
    transitionImageLayout(
        commandBuffer,
        m_image.image,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_ACCESS_TRANSFER_READ_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT
    );

    VkBufferImageCopy region = {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .imageOffset = {0, 0, 0},
        .imageExtent = {m_width, m_height, 1},
    };

    vkCmdCopyImageToBuffer(
        commandBuffer, m_image.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_stagingBuffer.buffer, 1, &region
    );

    transitionImageLayout(
        commandBuffer,
        m_image.image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_ACCESS_TRANSFER_READ_BIT,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    );
    VulkanContext::instance().endSingleTimeCommands(commandBuffer);

    VkDeviceSize imageSize = m_width * m_height * 4;
    std::vector<std::byte> imageData(imageSize);
    void* mappedData;
    if (vmaMapMemory(VulkanContext::instance().allocator(), m_stagingBuffer.allocation, &mappedData) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to map staging buffer memory for reading offscreen surface image data");
    }

    std::memcpy(imageData.data(), mappedData, imageData.size());
    vmaUnmapMemory(VulkanContext::instance().allocator(), m_stagingBuffer.allocation);

    return imageData;
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