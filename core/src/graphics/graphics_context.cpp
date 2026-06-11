#include <karin/graphics/graphics_context.h>

#include "canvas.h"
#include "platform.h"

namespace
{
using namespace karin;

karin::Rectangle intersectRects(const karin::Rectangle& a, const karin::Rectangle& b)
{
    float x1 = std::max(a.pos.x, b.pos.x);
    float y1 = std::max(a.pos.y, b.pos.y);
    float x2 = std::min(a.pos.x + a.size.width, b.pos.x + b.size.width);
    float y2 = std::min(a.pos.y + a.size.height, b.pos.y + b.size.height);

    if (x2 >= x1 && y2 >= y1)
    {
        return karin::Rectangle(Point(x1, y1), Size(x2 - x1, y2 - y1));
    }
    else
    {
        return karin::Rectangle(Point(0, 0), Size(0, 0));
    }
}
}

namespace karin
{
GraphicsContext::GraphicsContext(Canvas* canvas)
    : m_canvas(canvas)
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

void GraphicsContext::clip(Rectangle rect)
{
    Rectangle transformedRect = applyTransform(m_currentState.transform, rect);

    if (m_currentState.clipRect.has_value())
    {
        m_currentState.clipRect = intersectRects(m_currentState.clipRect.value(), transformedRect);
    }
    else
    {
        m_currentState.clipRect = transformedRect;
    }
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

void GraphicsContext::fillPath(Path path, const Pattern& pattern) const
{
    m_canvas->fillPath(std::move(path), pattern, m_currentState);
}

void GraphicsContext::drawPath(Path path, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_canvas->drawPath(std::move(path), pattern, strokeStyle, m_currentState);
}

void GraphicsContext::drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity) const
{
    m_canvas->drawImage(image, destRect, srcRect, opacity, m_currentState);
}

void GraphicsContext::drawText(const TextBlob& text, Point start, const Pattern& pattern) const
{
    m_canvas->drawText(text, start, pattern, m_currentState);
}
} // karin
