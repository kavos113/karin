#include "canvas.h"

#include <algorithm>
#include <utility>
#include <ranges>

namespace karin
{
void Canvas::fillRect(Rectangle rect, const Pattern& pattern, const GraphicsContext::State& state)
{
    DrawInstructionFillRect instruction{rect, pattern};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern,
    const GraphicsContext::State& state)
{
    DrawInstructionFillEllipse instruction{center, radiusX, radiusY, pattern};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern,
    const GraphicsContext::State& state)
{
    DrawInstructionFillRoundedRect instruction{rect, radiusX, radiusY, pattern};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::fillPath(Path path, const Pattern& pattern, const GraphicsContext::State& state)
{
    DrawInstructionFillPath instruction{std::move(path), pattern};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle,
    const GraphicsContext::State& state)
{
    DrawInstructionDrawLine instruction{start, end, pattern, strokeStyle};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle,
    const GraphicsContext::State& state)
{
    DrawInstructionDrawRect instruction{rect, pattern, strokeStyle};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state)
{
    DrawInstructionDrawEllipse instruction{center, radiusX, radiusY, pattern, strokeStyle};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state)
{
    DrawInstructionDrawRoundedRect instruction{rect, radiusX, radiusY, pattern, strokeStyle};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::drawPath(Path path, const Pattern& pattern, const StrokeStyle& strokeStyle,
                      const GraphicsContext::State& state)
{
    DrawInstructionDrawPath instruction{std::move(path), pattern, strokeStyle};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity,
    const GraphicsContext::State& state)
{
    DrawInstructionDrawImage instruction{std::move(image), destRect, srcRect, opacity};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::drawText(const TextBlob& text, Point start, const Pattern& pattern, const GraphicsContext::State& state)
{
    DrawInstructionDrawText instruction{text, start, pattern};
    m_drawInstructions.emplace_back(state, instruction);
}

void Canvas::paint(IPainter* painter, IFontRendererImpl* fontRenderer) const
{
    for (const auto& instruction : m_drawInstructions)
    {
        std::visit(
            [painter, fontRenderer, instruction]<typename T0>(const T0& inst)
            {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, DrawInstructionFillRect>)
                {
                    painter->fillRect(inst.rect, inst.pattern, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionFillEllipse>)
                {
                    painter->fillEllipse(inst.center, inst.radiusX, inst.radiusY, inst.pattern, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionFillRoundedRect>)
                {
                    painter->fillRoundedRect(inst.rect, inst.radiusX, inst.radiusY, inst.pattern, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionFillPath>)
                {
                    painter->fillPath(*inst.path.impl(), inst.pattern, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawLine>)
                {
                    painter->drawLine(inst.start, inst.end, inst.pattern, inst.strokeStyle, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawRect>)
                {
                    painter->drawRect(inst.rect, inst.pattern, inst.strokeStyle, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawEllipse>)
                {
                    painter->drawEllipse(inst.center, inst.radiusX, inst.radiusY, inst.pattern, inst.strokeStyle, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawRoundedRect>)
                {
                    painter->drawRoundedRect(inst.rect, inst.radiusX, inst.radiusY, inst.pattern, inst.strokeStyle, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawPath>)
                {
                    painter->drawPath(*inst.path.impl(), inst.pattern, inst.strokeStyle, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawImage>)
                {
                    painter->drawImage(inst.image, inst.destRect, inst.srcRect, inst.opacity, instruction.state);
                }
                else if constexpr (std::is_same_v<T, DrawInstructionDrawText>)
                {
                    fontRenderer->drawText(inst.text, inst.start, inst.pattern, instruction.state);
                }
            }, instruction.instruction
        );
    }
}

void Canvas::sortInstructions()
{
    std::ranges::stable_sort(m_drawInstructions, [](const DrawInstruction& a, const DrawInstruction& b)
    {
        return a.state.zIndex < b.state.zIndex;
    });
}
} // karin