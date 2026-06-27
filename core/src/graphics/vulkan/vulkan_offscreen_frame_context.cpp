#include "vulkan_offscreen_frame_context.h"

namespace karin
{
VulkanOffscreenFrameContext::VulkanOffscreenFrameContext(std::unique_ptr<VulkanOffscreenSurface> surface)
    : VulkanFrameContext(std::move(surface))
    , m_offscreenSurface(static_cast<VulkanOffscreenSurface*>(m_surface.get())) // safe to static cast
{
}

void VulkanOffscreenFrameContext::cleanup()
{
    VulkanFrameContext::cleanup();

    m_offscreenSurface = nullptr;
}

std::vector<std::byte> VulkanOffscreenFrameContext::getImageData() const
{
    return m_offscreenSurface->getImageData();
}
} // karin