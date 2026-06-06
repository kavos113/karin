#ifndef KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#define KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H

#include <memory>
#include <vector>

#include <karin/common/geometry/rectangle.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/transform2d.h>

#include "path.h"
#include "stroke_style.h"
#include "pattern.h"
#include "image.h"
#include "text_blob.h"

namespace karin
{

class ICanvas;
class IFontRendererImpl;
/**
 * GraphicsContext provides basic drawing operations(e.g., drawRect).
 *
 * This class is only created by Renderer.
 */
class GraphicsContext
{
private:
    explicit GraphicsContext(std::unique_ptr<ICanvas> canvas, IFontRendererImpl* fontRenderer);
    ~GraphicsContext();

    friend class WindowRenderer;
    friend class OffscreenRenderer;

public:
    struct State
    {
        Transform2D transform;
    };

    void save();
    void restore();
    void reset();
    void multiplyTransform(const Transform2D& transform);
    void setTransform(const Transform2D& transform);

    void fillRect(Rectangle rect, const Pattern& pattern, const Transform2D& transform = Transform2D()) const;
    void fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const Transform2D& transform = Transform2D()) const;
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const Transform2D& transform = Transform2D()) const;
    void fillPath(const Path& path, const Pattern& pattern, const Transform2D& transform = Transform2D()) const;

    void drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(), const Transform2D& transform = Transform2D()) const;
    void drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(), const Transform2D& transform = Transform2D()) const;
    void drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(), const Transform2D& transform = Transform2D()) const;
    void drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(), const Transform2D& transform = Transform2D()) const;
    void drawPath(const Path& path, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(), const Transform2D& transform = Transform2D()) const;

    void drawImage(Image image, Rectangle destRect, Rectangle srcRect = Rectangle(), float opacity = 1.0f, const Transform2D& transform = Transform2D()) const;
    void drawText(const TextBlob& text, Point start, const Pattern& pattern, const Transform2D& transform = Transform2D()) const;

private:
    std::unique_ptr<ICanvas> m_canvas;
    IFontRendererImpl* m_fontRenderer;

    State m_currentState;
    std::vector<State> m_stateStack;

    static constexpr size_t MAX_STATE_STACK_SIZE = 128;
};
} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
