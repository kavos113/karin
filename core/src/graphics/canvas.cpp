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

void Canvas::paint(IPainter* painter, IFontRendererImpl* fontRenderer) const
{
    for (const auto& instruction : m_drawInstructions)
    {
        std::visit(
            [painter, fontRenderer]<typename T0>(const T0& inst)
            {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, DrawInstructionFillRect>)
                {
                    painter->fillRect(inst.rect, inst.pattern, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionFillEllipse>)
                {
                    painter->fillEllipse(inst.center, inst.radiusX, inst.radiusY, inst.pattern, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionFillRoundedRect>)
                {
                    painter->fillRoundedRect(inst.rect, inst.radiusX, inst.radiusY, inst.pattern, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionFillPath>)
                {
                    painter->fillPath(inst.path.impl(), inst.pattern, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawLine>)
                {
                    painter->drawLine(inst.start, inst.end, inst.pattern, inst.strokeStyle, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawRect>)
                {
                    painter->drawRect(inst.rect, inst.pattern, inst.strokeStyle, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawEllipse>)
                {
                    painter->drawEllipse(inst.center, inst.radiusX, inst.radiusY, inst.pattern, inst.strokeStyle, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawRoundedRect>)
                {
                    painter->drawRoundedRect(inst.rect, inst.radiusX, inst.radiusY, inst.pattern, inst.strokeStyle, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawPath>)
                {
                    painter->drawPath(inst.path.impl(), inst.pattern, inst.strokeStyle, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawImage>)
                {
                    painter->drawImage(inst.image, inst.destRect, inst.srcRect, inst.opacity, inst.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawText>)
                {
                    fontRenderer->drawText(inst.text, inst.start, inst.pattern, inst.state);
                }
            }, instruction
        );
    }
}
} // karin