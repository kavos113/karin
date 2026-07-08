#ifndef KARIN_GRAPHICS_OFFSCREEN_RENDERER_H
#define KARIN_GRAPHICS_OFFSCREEN_RENDERER_H

#include <cstddef>
#include <cstdint>

#include <vector>

#include "renderer_base.h"

namespace karin
{
class IOffscreenRendererTarget;

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
    IOffscreenRendererTarget *m_offscreenTarget;
};
} // karin

#endif //KARIN_GRAPHICS_OFFSCREEN_RENDERER_H
