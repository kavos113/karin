#include <karin/graphics/offscreen_renderer.h>

#include "offscreen_renderer_impl.h"
#include "renderer_impl.h"
#include "platform.h"

namespace karin
{
OffscreenRenderer::OffscreenRenderer(uint32_t width, uint32_t height)
{
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
        return;
    }

    GraphicsContext context(m_impl.get());

    for (const auto& command : m_drawCommands)
    {
        command(context);
    }

    m_impl->endDraw();
}
}
