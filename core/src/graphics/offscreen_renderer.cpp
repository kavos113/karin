#include <karin/graphics/offscreen_renderer.h>

#include "offscreen_renderer_impl.h"
#include "renderer_impl.h"
#include "platform.h"
#include "canvas.h"

namespace karin
{
OffscreenRenderer::OffscreenRenderer(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0)
    {
        throw std::invalid_argument("OffscreenRenderer cannot have zero width or height");
    }

    // TODO: 整数のsizeでよくないか
    auto impl = createOffscreenRendererImpl(Size(width, height));
    m_offscreenImpl = impl.offscreenRendererImpl;
    m_impl = std::move(impl.rendererImpl);
}

OffscreenRenderer::~OffscreenRenderer() = default;

std::vector<std::byte> OffscreenRenderer::getImageData() const
{
    return m_offscreenImpl->getImageData();
}

void OffscreenRenderer::draw() const
{
    bool res = m_impl->beginDraw();
    if (!res)
    {
        std::cerr << "Failed to begin drawing on offscreen surface" << std::endl;
        return;
    }

    Canvas canvas;
    GraphicsContext context(&canvas);
    for (const auto& command : m_drawCommands)
    {
        command(context);
    }

    auto painter = createPainter(m_impl.get());
    auto fontRenderer = m_impl->fontRenderer();
    canvas.paint(painter.get(), fontRenderer);

    m_impl->endDraw();
}
}
