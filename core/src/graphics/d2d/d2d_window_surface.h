#ifndef GRAPHICS_D2D_D2D_WINDOW_SURFACE_H
#define GRAPHICS_D2D_D2D_WINDOW_SURFACE_H

#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d2d1_1.h>

#include <karin/common/geometry/size.h>
#include "d2d_surface.h"
#include "d2d_consts.h"

namespace karin
{
class D2DWindowSurface : public ID2DSurface
{
public:
    D2DWindowSurface(HWND hwnd);
    ~D2DWindowSurface() override = default;

    void cleanUp() override;
    void present() override;
    void resize(Size size) override;

    Microsoft::WRL::ComPtr<ID2D1Bitmap> getTargetBitmap() const override;
    void setDeviceContext(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext) override;

private:
    void createSwapChain();
    void acquireBackBuffer();

    HWND m_hwnd;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISurface> m_backBuffer;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;

    const D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        DEFAULT_DPI, DEFAULT_DPI
    );
};
} // karin

#endif //GRAPHICS_D2D_D2D_WINDOW_SURFACE_H
