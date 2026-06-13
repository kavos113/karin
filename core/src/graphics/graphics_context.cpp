#include <karin/graphics/graphics_context.h>

#include "canvas.h"

namespace
{
using namespace karin;

Rectangle intersectRects(const Rectangle& a, const Rectangle& b)
{
    float x1 = std::max(a.pos.x, b.pos.x);
    float y1 = std::max(a.pos.y, b.pos.y);
    float x2 = std::min(a.pos.x + a.size.width, b.pos.x + b.size.width);
    float y2 = std::min(a.pos.y + a.size.height, b.pos.y + b.size.height);

    if (x2 >= x1 && y2 >= y1)
    {
        return Rectangle(Point(x1, y1), Size(x2 - x1, y2 - y1));
    }
    else
    {
        return Rectangle(Point(0, 0), Size(0, 0));
    }
}
}

namespace karin
{
GraphicsContext::GraphicsContext()
{
    m_canvas = std::make_unique<Canvas>();
    m_stateStack.reserve(MAX_STACK_SIZE);
    m_layerStack.reserve(MAX_STACK_SIZE);
}

GraphicsContext::~GraphicsContext() = default;

Canvas* GraphicsContext::canvas() const
{
    return m_canvas.get();
}

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

void GraphicsContext::setZIndex(float zIndex)
{
    m_currentState.zIndex = zIndex;
}

void GraphicsContext::translateZ(float dz)
{
    m_currentState.zIndex += dz;
}

void GraphicsContext::setAlpha(float alpha)
{
    m_currentState.alpha = alpha;
}

void GraphicsContext::multiplyAlpha(float alpha)
{
    m_currentState.alpha *= alpha;
}

void GraphicsContext::fillRect(Rectangle rect, const Pattern& pattern) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->fillRect(rect, pattern, m_currentState);
    }
    else
    {
        m_canvas->fillRect(rect, pattern, m_currentState);
    }
}

void GraphicsContext::fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->fillEllipse(center, radiusX, radiusY, pattern, m_currentState);
    }
    else
    {
        m_canvas->fillEllipse(center, radiusX, radiusY, pattern, m_currentState);
    }
}

void GraphicsContext::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->fillRoundedRect(rect, radiusX, radiusY, pattern, m_currentState);
    }
    else
    {
        m_canvas->fillRoundedRect(rect, radiusX, radiusY, pattern, m_currentState);
    }
}

void GraphicsContext::drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->drawLine(start, end, pattern, strokeStyle, m_currentState);
    }
    else
    {
        m_canvas->drawLine(start, end, pattern, strokeStyle, m_currentState);
    }
}

void GraphicsContext::drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->drawRect(rect, pattern, strokeStyle, m_currentState);
    }
    else
    {
        m_canvas->drawRect(rect, pattern, strokeStyle, m_currentState);
    }
}

void GraphicsContext::drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->drawEllipse(center, radiusX, radiusY, pattern, strokeStyle, m_currentState);
    }
    else
    {
        m_canvas->drawEllipse(center, radiusX, radiusY, pattern, strokeStyle, m_currentState);
    }
}

void GraphicsContext::drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->drawRoundedRect(rect, radiusX, radiusY, pattern, strokeStyle, m_currentState);
    }
    else
    {
        m_canvas->drawRoundedRect(rect, radiusX, radiusY, pattern, strokeStyle, m_currentState);
    }
}

void GraphicsContext::fillPath(Path path, const Pattern& pattern) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->fillPath(std::move(path), pattern, m_currentState);
    }
    else
    {
        m_canvas->fillPath(std::move(path), pattern, m_currentState);
    }
}

void GraphicsContext::drawPath(Path path, const Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->drawPath(std::move(path), pattern, strokeStyle, m_currentState);
    }
    else
    {
        m_canvas->drawPath(std::move(path), pattern, strokeStyle, m_currentState);
    }
}

void GraphicsContext::drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->drawImage(std::move(image), destRect, srcRect, opacity, m_currentState);
    }
    else
    {
        m_canvas->drawImage(std::move(image), destRect, srcRect, opacity, m_currentState);
    }
}

void GraphicsContext::drawText(const TextBlob& text, Point start, const Pattern& pattern) const
{
    if (!m_layerStack.empty())
    {
        const Layer& currentLayer = m_layerStack.back();
        currentLayer.canvas->drawText(text, start, pattern, m_currentState);
    }
    else
    {
        m_canvas->drawText(text, start, pattern, m_currentState);
    }
}

void GraphicsContext::beginLayer(Rectangle bounds, float alpha)
{
    Layer newLayer{
        .canvas = std::make_unique<Canvas>(),
        .bounds = bounds,
        .alpha = alpha
    };
    m_layerStack.push_back(std::move(newLayer));
}

void GraphicsContext::endLayer()
{
    if (!m_layerStack.empty())
    {
        Layer layer = std::move(m_layerStack.back());
        m_layerStack.pop_back();

        m_canvas->drawLayer(layer.bounds, layer.alpha, *layer.canvas, m_currentState);
    }
}
} // karin
