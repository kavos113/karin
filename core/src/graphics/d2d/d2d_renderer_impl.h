#ifndef SRC_GRAPHICS_GRAPHICS_D2D_D2D_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_D2D_D2D_RENDERER_IMPL_H

#include "d2d_color.h"
#include "d2d_consts.h"

#include <d2d1_1.h>

#include <renderer_impl.h>

#include "d2d_device_resources.h"
#include "d2d_window_surface.h"
#include "d2d_font_renderer.h"
#include "d2d_surface.h"

namespace karin
{
class D2DRendererImpl : public IRendererImpl
{
public:
    D2DRendererImpl(std::unique_ptr<ID2DSurface> surface);
    ~D2DRendererImpl() override = default;

    void cleanUp() override;

    bool beginDraw() override;
    void endDraw() override;
    void resize(Size size) override;

    void setClearColor(const Color& color) override
    {
        m_clearColor = toD2DColor(color);
    }

    Image createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height) override
    {
        return m_deviceResources->createImage(data, width, height);
    }

    void startResizing() override {}
    void finishResizing() override {}

    [[nodiscard]] Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext() const
    {
        return m_deviceContext;
    }

    [[nodiscard]] D2DDeviceResources* deviceResources() const
    {
        return m_deviceResources.get();
    }

    IFontRendererImpl* fontRenderer() override
    {
        return m_fontRenderer.get();
    }

private:
    void setTargetBitmap() const;

    std::unique_ptr<ID2DSurface> m_surface;
    std::unique_ptr<D2DDeviceResources> m_deviceResources;
    std::unique_ptr<D2DFontRenderer> m_fontRenderer;

    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;

    D2D1_COLOR_F m_clearColor = D2D1::ColorF(D2D1::ColorF::White);
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_D2D_RENDERER_IMPL_H
