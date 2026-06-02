#include "d2d_offscreen_renderer_impl.h"

namespace karin
{
D2DOffscreenRendererImpl::D2DOffscreenRendererImpl(std::unique_ptr<ID2DSurface> surface, D2DOffscreenSurface* offscreenSurface)
    : D2DRendererImpl(std::move(surface))
    , m_offscreenSurface(offscreenSurface)
{
}

std::vector<std::byte> D2DOffscreenRendererImpl::getImageData() const
{
    return m_offscreenSurface->getImageData();
}
} // karin