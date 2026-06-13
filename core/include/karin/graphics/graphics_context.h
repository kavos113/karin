#ifndef KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#define KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H

#include <vector>
#include <optional>
#include <memory>

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

class Canvas;
/**
 * GraphicsContext provides basic drawing operations(e.g., drawRect).
 *
 * This class is only created by Renderer.
 */
class GraphicsContext
{
private:
    GraphicsContext();
    ~GraphicsContext();

    Canvas *canvas() const;

    friend class WindowRenderer;
    friend class OffscreenRenderer;

public:
    struct State
    {
        Transform2D transform;
        std::optional<Rectangle> clipRect = std::nullopt;
        float zIndex = 0.0f;
        float alpha = 1.0f;
    };

    void save();
    void restore();
    void reset();
    void multiplyTransform(const Transform2D& transform);
    void clip(Rectangle rect);
    void setZIndex(float zIndex);
    void translateZ(float dz);
    void setAlpha(float alpha);
    void multiplyAlpha(float alpha);

    template<typename Func>
    void withSave(Func func)
    {
        save();
        try
        {
            func();
        }
        catch (...)
        {
            restore();
            throw;
        }
        restore();
    }

    void fillRect(Rectangle rect, const Pattern& pattern) const;
    void fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern) const;
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern) const;
    void fillPath(Path path, const Pattern& pattern) const;

    void drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()) const;
    void drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()) const;
    void drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()) const;
    void drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()) const;
    void drawPath(Path path, const Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()) const;

    void drawImage(Image image, Rectangle destRect, Rectangle srcRect = Rectangle(), float opacity = 1.0f) const;
    void drawText(const TextBlob& text, Point start, const Pattern& pattern) const;

    void saveLayer(Rectangle bounds, float alpha = 1.0f);
    void restoreLayer();

private:
    std::unique_ptr<Canvas> m_canvas;

    State m_currentState;
    std::vector<State> m_stateStack;

    struct Layer
    {
        std::unique_ptr<Canvas> canvas;
        Rectangle bounds;
        float alpha;
    };
    std::vector<Layer> m_layerStack;

    static constexpr size_t MAX_STACK_SIZE = 128;
};
} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
