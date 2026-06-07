#ifndef CORE_SRC_GRAPHICS_CANVAS_H
#define CORE_SRC_GRAPHICS_CANVAS_H

#include <karin/common/geometry/rectangle.h>
#include <karin/common/geometry/point.h>
#include <karin/graphics/path.h>
#include <karin/graphics/stroke_style.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/image.h>
#include <karin/graphics/graphics_context.h>

namespace karin
{
class ICanvas
{
public:
    virtual ~ICanvas() = default;

    virtual void fillRect(Rectangle rect, const Pattern& pattern, const GraphicsContext::State& state) = 0;
    virtual void fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state) = 0;
    virtual void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state) = 0;
    virtual void fillPath(const PathImpl& path, const Pattern& pattern, const GraphicsContext::State& state) = 0;

    virtual void drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) = 0;
    virtual void drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) = 0;
    virtual void drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) = 0;
    virtual void drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) = 0;
    virtual void drawPath(const PathImpl& path, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) = 0;

    virtual void drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity, const GraphicsContext::State& state) = 0;
};
} // karin

#endif //CORE_SRC_GRAPHICS_CANVAS_H
