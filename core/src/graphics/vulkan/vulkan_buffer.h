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
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_BUFER_H
