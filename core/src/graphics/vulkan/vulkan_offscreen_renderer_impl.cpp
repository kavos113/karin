#include "vulkan_offscreen_renderer_impl.h"

namespace karin
{
VulkanOffscreenRendererImpl::VulkanOffscreenRendererImpl(std::unique_ptr<IVulkanSurface> surface, VulkanOffscreenSurface* offscreenSurface)
    : VulkanRendererImpl(std::move(surface))
    , m_offscreenSurface(offscreenSurface)
{
}

std::vector<std::byte> VulkanOffscreenRendererImpl::getImageData() const
{
    return m_offscreenSurface->getImageData();
}
} // karin