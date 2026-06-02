#ifndef CORE_KARIN_GRAPHICS_OFFSCREEN_RENDERER_H
#define CORE_KARIN_GRAPHICS_OFFSCREEN_RENDERER_H

#include "renderer_base.h"

namespace karin
{
class IOffscreenRendererImpl;

class OffscreenRenderer : public RendererBase
{
public:
    OffscreenRenderer(uint32_t width, uint32_t height);
    ~OffscreenRenderer() override;

     /**
      * Get the rendered image data. The format of the data is R8G8B8A8_UNORM
      *
      * @return a vector of bytes containing the image data.
      */
    std::vector<std::byte> getImageData() const;

    void draw() const;

private:
    IOffscreenRendererImpl *m_offscreenImpl;
};
} // karin

#endif //CORE_KARIN_GRAPHICS_OFFSCREEN_RENDERER_H
