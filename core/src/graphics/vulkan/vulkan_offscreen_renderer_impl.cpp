#include "vulkan_offscreen_renderer_impl.h"

namespace karin
{
VulkanOffscreenRendererImpl::VulkanOffscreenRendererImpl(std::unique_ptr<VulkanOffscreenSurface> surface)
    : VulkanRendererImpl(std::move(surface))
    , m_offscreenSurface(static_cast<VulkanOffscreenSurface*>(m_surface.get())) // Safe to static_cast
{
}

void VulkanOffscreenRendererImpl::cleanUp()
{
    VulkanRendererImpl::cleanUp();

    m_offscreenSurface = nullptr;
}

std::vector<std::byte> VulkanOffscreenRendererImpl::getImageData() const
{
    return m_offscreenSurface->getImageData();
}
} // karin