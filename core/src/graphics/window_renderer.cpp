#include <karin/graphics/window_renderer.h>

#include "platform.h"
#include "renderer_impl.h"
#include "canvas.h"

#include <vector>

namespace karin
{
WindowRenderer::WindowRenderer(Window* window)
    : m_window(window)
{
    m_impl = createRendererImpl(window->handle());
}

WindowRenderer::~WindowRenderer() = default;

void WindowRenderer::update()
{
    if (m_updated)
    {
        return;
    }

    m_window->addPaintCallback(
        [this]
        {
            bool res = m_impl->beginDraw();
            if (!res)
            {
                return false;
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

            return true;
        }
    );

    m_window->addResizeCallback(
        [this](Size size)
        {
            if (size.width == 0 || size.height == 0)
            {
                return;
            }
            m_impl->resize(size);

            m_window->invalidate();
        }
    );

    m_window->addStartResizeCallback(
        [this]
        {
            m_impl->startResizing();
        }
    );

    m_window->addFinishResizeCallback(
        [this]
        {
            m_impl->finishResizing();
        }
    );

    m_updated = true;
}
} // karin