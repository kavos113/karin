#include "canvas.h"

#include <utility>

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
    DrawInstructionFillEllipse instruction{center, radiusX, radiusY, pattern, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern,
    const GraphicsContext::State& state)
{
    DrawInstructionFillRoundedRect instruction{rect, radiusX, radiusY, pattern, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::fillPath(Path path, const Pattern& pattern, const GraphicsContext::State& state)
{
    DrawInstructionFillPath instruction{std::move(path), pattern, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle,
    const GraphicsContext::State& state)
{
    DrawInstructionDrawLine instruction{start, end, pattern, strokeStyle, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle,
    const GraphicsContext::State& state)
{
    DrawInstructionDrawRect instruction{rect, pattern, strokeStyle, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state)
{
    DrawInstructionDrawEllipse instruction{center, radiusX, radiusY, pattern, strokeStyle, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state)
{
    DrawInstructionDrawRoundedRect instruction{rect, radiusX, radiusY, pattern, strokeStyle, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::drawPath(Path path, const Pattern& pattern, const StrokeStyle& strokeStyle,
                      const GraphicsContext::State& state)
{
    DrawInstructionDrawPath instruction{std::move(path), pattern, strokeStyle, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity,
    const GraphicsContext::State& state)
{
    DrawInstructionDrawImage instruction{std::move(image), destRect, srcRect, opacity, state};
    m_drawInstructions.emplace_back(instruction);
}

void Canvas::drawText(const TextBlob& text, Point start, const Pattern& pattern, const GraphicsContext::State& state)
{
    DrawInstructionDrawText instruction{text, start, pattern, state};
    m_drawInstructions.emplace_back(instruction);
}
} // karin