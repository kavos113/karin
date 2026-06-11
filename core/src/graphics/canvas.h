#ifndef CORE_SRC_GRAPHICS_CANVAS_H
#define CORE_SRC_GRAPHICS_CANVAS_H

#include <vector>

#include <karin/common/geometry/rectangle.h>
#include <karin/common/geometry/point.h>
#include <karin/graphics/path.h>
#include <karin/graphics/stroke_style.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/image.h>
#include <karin/graphics/graphics_context.h>
#include "draw_instruction.h"

namespace karin
{
class Canvas
{
public:
    void fillRect(Rectangle rect, const Pattern& pattern, const GraphicsContext::State& state);
    void fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state);
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state);
    void fillPath(Path path, const Pattern& pattern, const GraphicsContext::State& state);
    void drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state);
    void drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state);
    void drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state);
    void drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state);
    void drawPath(Path path, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state);
    void drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity, const GraphicsContext::State& state);
    void drawText(const TextBlob& text, Point start, const Pattern& pattern, const GraphicsContext::State& state);
    
private:
    std::vector<DrawInstruction> m_drawInstructions;
};
} // karin

#endif //CORE_SRC_GRAPHICS_CANVAS_H
