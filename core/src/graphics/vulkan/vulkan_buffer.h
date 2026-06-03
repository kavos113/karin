#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_BUFER_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_BUFER_H

#include <vulkan/vulkan.h>
#include "vma.h"

#include "vulkan_context.h"

namespace karin
{
template <typename T>
struct VulkanBuffer
{
    VulkanBuffer() = default;
    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;

    VulkanBuffer(VulkanBuffer&& other) noexcept
        : buffer(other.buffer), allocation(other.allocation), mappedData(other.mappedData)
    {
        other.buffer = VK_NULL_HANDLE;
        other.allocation = VK_NULL_HANDLE;
        other.mappedData = nullptr;
    }
    VulkanBuffer& operator=(VulkanBuffer&& other) noexcept
    {
        if (this != &other)
        {
            cleanup();

            buffer = other.buffer;
            allocation = other.allocation;
            mappedData = other.mappedData;

            other.buffer = VK_NULL_HANDLE;
            other.allocation = VK_NULL_HANDLE;
            other.mappedData = nullptr;
        }
        return *this;
    }

    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    T *mappedData = nullptr;

    VkResult create(const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocInfo)
    {
        VmaAllocationInfo memoryInfo;
        VkResult result = vmaCreateBuffer(
            VulkanContext::instance().allocator(), &bufferInfo, &allocInfo, &buffer, &allocation, &memoryInfo
        );
        if (result != VK_SUCCESS)
        {
            return result;
        }

        mappedData = static_cast<T*>(memoryInfo.pMappedData);

        return VK_SUCCESS;
    }

    void cleanup()
    {
        if (buffer != VK_NULL_HANDLE)
        {
            vmaDestroyBuffer(VulkanContext::instance().allocator(), buffer, allocation);

            buffer = VK_NULL_HANDLE;
            allocation = VK_NULL_HANDLE;
            mappedData = nullptr;
        }
    }
};

struct VulkanImage
{
    VkImage image = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;

    // VkImageViewCreateInfo.image will be set in this function, so the caller doesn't need to worry about it
    VkResult create(const VkImageCreateInfo& imageInfo, const VmaAllocationCreateInfo& allocInfo, VkImageViewCreateInfo& viewInfo)
    {
        VmaAllocationInfo memoryInfo;
        auto t = VulkanContext::instance().allocator();
        VkResult result = vmaCreateImage(
            VulkanContext::instance().allocator(), &imageInfo, &allocInfo, &image, &allocation, &memoryInfo
        );
        if (result != VK_SUCCESS)
        {
            return result;
        }

        viewInfo.image = image;

        if (vkCreateImageView(VulkanContext::instance().device(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            vmaDestroyImage(VulkanContext::instance().allocator(), image, allocation);
            image = VK_NULL_HANDLE;
            allocation = VK_NULL_HANDLE;

            return VK_ERROR_INITIALIZATION_FAILED;
        }

        return VK_SUCCESS;
    }

    void cleanup()
    {
        if (imageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(VulkanContext::instance().device(), imageView, nullptr);
            imageView = VK_NULL_HANDLE;
        }

        if (image != VK_NULL_HANDLE)
        {
            vmaDestroyImage(VulkanContext::instance().allocator(), image, allocation);
            image = VK_NULL_HANDLE;
            allocation = VK_NULL_HANDLE;
        }
    }
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_BUFER_H
