#include <karin/graphics/graphics_context.h>

#include "platform.h"
#include "canvas.h"
#include "font_renderer_impl.h"

namespace karin
{
GraphicsContext::GraphicsContext(std::unique_ptr<ICanvas> canvas, IFontRendererImpl* fontRenderer)
    : m_canvas(std::move(canvas)), m_fontRenderer(fontRenderer)
{
    m_stateStack.reserve(MAX_STATE_STACK_SIZE);
}

GraphicsContext::~GraphicsContext() = default;

void GraphicsContext::save()
{
    m_stateStack.push_back(m_currentState);
}

void GraphicsContext::restore()
{
    if (!m_stateStack.empty())
    {
        m_currentState = m_stateStack.back();
        m_stateStack.pop_back();
    }
}

void GraphicsContext::reset()
{
    m_currentState = State();
    m_stateStack.clear();
}

void GraphicsContext::multiplyTransform(const Transform2D& transform)
{
    m_currentState.transform = m_currentState.transform * transform;
}

void GraphicsContext::setTransform(const Transform2D& transform)
{
    m_currentState.transform = transform;
}

void GraphicsContext::fillRect(Rectangle rect, const Pattern& pattern) const
{
    m_canvas->fillRect(rect, pattern, m_currentState);
}

void GraphicsContext::fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern) const
{
    m_canvas->fillEllipse(center, radiusX, radiusY, pattern, m_currentState);
}

void GraphicsContext::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern) const
{
    m_canvas->fillRoundedRect(rect, radiusX, radiusY, pattern, m_currentState);
}

void GraphicsContext::drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_canvas->drawLine(start, end, pattern, strokeStyle, m_currentState);
}

void GraphicsContext::drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_canvas->drawRect(rect, pattern, strokeStyle, m_currentState);
}

void GraphicsContext::drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_canvas->drawEllipse(center, radiusX, radiusY, pattern, strokeStyle, m_currentState);
}

void GraphicsContext::drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_canvas->drawRoundedRect(rect, radiusX, radiusY, pattern, strokeStyle, m_currentState);
}

void GraphicsContext::fillPath(const Path& path, const Pattern& pattern) const
{
    m_canvas->fillPath(*path.impl(), pattern, m_currentState);
}

void GraphicsContext::drawPath(const Path& path, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_canvas->drawPath(*path.impl(), pattern, strokeStyle, m_currentState);
}

void GraphicsContext::drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity) const
{
    m_canvas->drawImage(image, destRect, srcRect, opacity, m_currentState);
}

void GraphicsContext::drawText(const TextBlob& text, Point start, const Pattern& pattern) const
{
    m_fontRenderer->drawText(text, start, pattern, m_currentState);
}
} // karin
