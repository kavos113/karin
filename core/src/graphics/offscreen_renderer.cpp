#include <karin/graphics/offscreen_renderer.h>

#include "offscreen_renderer_impl.h"

karin::OffscreenRenderer::OffscreenRenderer(uint32_t width, uint32_t height)
{
}

karin::OffscreenRenderer::~OffscreenRenderer() = default;

std::vector<std::byte> karin::OffscreenRenderer::getImageData() const
{
    return m_offscreenImpl->getImageData();
}
