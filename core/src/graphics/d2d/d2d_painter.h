#ifndef GRAPHICS_D2D_D2D_PAINTER_H
#define GRAPHICS_D2D_D2D_PAINTER_H

#include <d2d1_1.h>
#include <wrl/client.h>

#include <karin/common/geometry/rectangle.h>
#include <karin/common/geometry/point.h>
#include <karin/graphics/stroke_style.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/image.h>
#include <painter.h>
#include "d2d_device_resources.h"


namespace karin
{
class D2DPainter : public IPainter
{
public:
    D2DPainter(
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
        D2DDeviceResources* deviceResources
    );

    ~D2DPainter() override = default;

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
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
    D2DDeviceResources* m_deviceResources;
};
} // karin

#endif //GRAPHICS_D2D_D2D_PAINTER_H
