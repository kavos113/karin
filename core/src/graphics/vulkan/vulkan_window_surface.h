#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H

#include <cstdint>

#include <vector>

#include <vulkan/vulkan.h>

#include <karin/system/window.h>
#include "vulkan_surface.h"

namespace karin
{
class VulkanWindowSurface : public IVulkanSurface
{
public:
    VulkanWindowSurface(Window::NativeHandle nativeHandle);
    ~VulkanWindowSurface() override = default;

    void cleanup() override;
    void resize() override;

    bool prepareNextImage(VkSemaphore semaphore) override;
    void beforeRender(VkCommandBuffer commandBuffer) override;
    void endRender(VkCommandBuffer commandBuffer) override;
    bool present(VkSemaphore waitSemaphore) const override;

    VkExtent2D extent() const override
    {
        return m_swapChainExtent;
    }

    VkFormat format() const override
    {
        return m_swapChainImageFormat;
    }

    VkImageView currentImageView() const override
    {
        return m_swapChainImageViews[m_imageIndex];
    }

    void startResizing() override
    {
        m_isResizing = true;
    }

    void finishResizing() override
    {
        m_isResizing = false;
    }

private:
    void createSurface();
    void createSwapChain(bool isRecreating);
    void createImageView();

    Window::NativeHandle m_window;

    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D m_swapChainExtent = {};

    std::vector<VkSwapchainKHR> m_oldSwapChains;
    bool m_isResizing = false;
    uint32_t m_imageIndex = 0;
};
} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H