#include "d2d_offscreen_renderer_impl.h"

namespace karin
{
D2DOffscreenRendererImpl::D2DOffscreenRendererImpl(std::unique_ptr<D2DOffscreenSurface> surface)
{
    m_surface = surface.get();
    D2DRendererImpl(std::move(surface));
}

std::vector<std::byte> D2DOffscreenRendererImpl::getImageData() const
{
    return m_surface->getImageData();
}
} // karin