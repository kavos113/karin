#ifndef CORE_SRC_GRAPHICS_D2D_D2D_OFFSCREEN_RENDERER_IMPL_H
#define CORE_SRC_GRAPHICS_D2D_D2D_OFFSCREEN_RENDERER_IMPL_H

#include <memory>
#include <vector>

#include <offscreen_renderer_impl.h>
#include "d2d_offscreen_surface.h"
#include "d2d_surface.h"
#include "d2d_renderer_impl.h"

namespace karin
{
class D2DOffscreenRendererImpl : public D2DRendererImpl, public IOffscreenRendererImpl
{
public:
    D2DOffscreenRendererImpl(std::unique_ptr<ID2DSurface> surface, D2DOffscreenSurface *offscreenSurface);
    ~D2DOffscreenRendererImpl() override = default;

    std::vector<std::byte> getImageData() const override;

private:
    D2DOffscreenSurface *m_offscreenSurface;
};
} // karin

#endif //CORE_SRC_GRAPHICS_D2D_D2D_OFFSCREEN_RENDERER_IMPL_H
