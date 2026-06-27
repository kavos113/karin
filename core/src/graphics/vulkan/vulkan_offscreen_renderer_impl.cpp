#include "vulkan_offscreen_renderer_impl.h"

namespace karin
{
VulkanOffscreenRendererImpl::VulkanOffscreenRendererImpl(std::unique_ptr<VulkanOffscreenFrameContext> surface)
    : VulkanRendererImpl(std::move(surface))
    , m_offscreenContext(static_cast<VulkanOffscreenFrameContext*>(m_frameContext.get())) // safe to static cast
{
}

void VulkanOffscreenRendererImpl::cleanUp()
{
    VulkanRendererImpl::cleanUp();

    m_offscreenContext = nullptr;
}

std::vector<std::byte> VulkanOffscreenRendererImpl::getImageData() const
{
    return m_offscreenContext->getImageData();
}
} // karin