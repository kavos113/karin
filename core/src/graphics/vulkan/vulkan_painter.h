#ifndef GRAPHICS_VULKAN_VULKAN_PAINTER_H
#define GRAPHICS_VULKAN_VULKAN_PAINTER_H

#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/stroke_style.h>
#include <karin/graphics/image.h>
#include <karin/graphics/graphics_context.h>
#include <painter.h>
#include <path_impl.h>
#include "vulkan_renderer_impl.h"

namespace karin
{
class VulkanPainter : public IPainter
{
public:
    explicit VulkanPainter(VulkanRendererImpl* renderer);
    ~VulkanPainter() override = default;

    void fillRect(Rectangle rect, const Pattern& pattern, const GraphicsContext::State& state) override;
    void fillEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state) override;
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state) override;
    void fillPath(const PathImpl& path, const Pattern& pattern, const GraphicsContext::State& state) override;
    void fillBoxShadow(Rectangle rect, Color color, float blurRadius, float spreadRadius, const GraphicsContext::State& state) override;

    void drawLine(Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) override;
    void drawRect(Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) override;
    void drawEllipse(Point center, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) override;
    void drawRoundedRect(Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) override;
    void drawPath(const PathImpl& path, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state) override;

    void drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity, const GraphicsContext::State& state) override;

    void pushLayer(Rectangle bounds, float alpha, const GraphicsContext::State& state) override;
    void popLayer() override;

private:
    VulkanRendererImpl* m_renderer;

    static constexpr int CAP_ROUND_SEGMENTS = 8;
    static constexpr int ELLIPSE_SEGMENTS = 32;
};
} // karin

#endif //GRAPHICS_VULKAN_VULKAN_PAINTER_H
