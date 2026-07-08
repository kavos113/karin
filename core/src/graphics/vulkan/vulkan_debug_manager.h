#ifndef GRAPHICS_VULKAN_VULKAN_DEBUG_MANAGER_H
#define GRAPHICS_VULKAN_VULKAN_DEBUG_MANAGER_H

#include <vulkan/vulkan.h>

namespace karin
{
class VulkanDebugManager
{
public:
    explicit VulkanDebugManager(VkInstance instance);
    ~VulkanDebugManager();

    void cleanup(VkInstance instance);
    void addDebugSupportToInstance(
        VkInstanceCreateInfo& createInfo, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo
    ) const;

private:
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
};
} // karin

#endif //GRAPHICS_VULKAN_VULKAN_DEBUG_MANAGER_H
