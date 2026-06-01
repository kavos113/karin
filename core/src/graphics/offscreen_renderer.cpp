#include <karin/graphics/offscreen_renderer.h>

#include "offscreen_renderer_impl.h"
#include "renderer_impl.h"

namespace karin
{
OffscreenRenderer::OffscreenRenderer(uint32_t width, uint32_t height)
{
}

OffscreenRenderer::~OffscreenRenderer() = default;

std::vector<std::byte> OffscreenRenderer::getImageData() const
{
    return m_offscreenImpl->getImageData();
}

}