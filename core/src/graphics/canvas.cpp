#include "canvas.h"

namespace karin
{
void Canvas::fillRect(Rectangle rect, const Pattern& pattern, const GraphicsContext::State& state)
{
    DrawInstructionFillRect instruction{rect, pattern, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern,
    const GraphicsContext::State& state)
{
}

void Canvas::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern,
    const GraphicsContext::State& state)
{
}

void Canvas::fillPath(const Path& path, const Pattern& pattern, const GraphicsContext::State& state)
{
}

void Canvas::drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle,
    const GraphicsContext::State& state)
{
}

void Canvas::drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle,
    const GraphicsContext::State& state)
{
}

void Canvas::drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state)
{
}

void Canvas::drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state)
{
}

void Canvas::drawPath(const Path& path, const Pattern& pattern, const StrokeStyle& strokeStyle,
    const GraphicsContext::State& state)
{
}

void Canvas::drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity,
    const GraphicsContext::State& state)
{
}

void Canvas::drawText(const TextBlob& text, Point start, const Pattern& pattern, const GraphicsContext::State& state)
{
}
} // karin