#ifndef GRAPHICS_D2D_D2D_OFFSCREEN_RENDERER_IMPL_H
#define GRAPHICS_D2D_D2D_OFFSCREEN_RENDERER_IMPL_H

#include <cstddef>

#include <memory>
#include <vector>

#include <offscreen_renderer_target.h>
#include "d2d_offscreen_surface.h"
#include "d2d_renderer_impl.h"

namespace karin
{
class D2DOffscreenRendererImpl : public D2DRendererImpl, public IOffscreenRendererTarget
{
public:
    D2DOffscreenRendererImpl(std::unique_ptr<D2DOffscreenSurface> surface);
    ~D2DOffscreenRendererImpl() override = default;

    void cleanUp() override;

    std::vector<std::byte> getImageData() const override;

private:
    D2DOffscreenSurface *m_offscreenSurface;
};
} // karin

#endif //GRAPHICS_D2D_D2D_OFFSCREEN_RENDERER_IMPL_H
