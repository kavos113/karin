#include "d2d_offscreen_renderer_impl.h"

namespace karin
{
D2DOffscreenRendererImpl::D2DOffscreenRendererImpl(std::unique_ptr<D2DOffscreenSurface> surface)
    : D2DRendererImpl(std::move(surface))
    , m_offscreenSurface(static_cast<D2DOffscreenSurface*>(m_surface.get())) // Safe to static_cast
{
}

std::vector<std::byte> D2DOffscreenRendererImpl::getImageData() const
{
    return m_offscreenSurface->getImageData();
}
} // karin