#ifndef CORE_SRC_GRAPHICS_D2D_D2D_OFFSCREEN_SURFACE_H
#define CORE_SRC_GRAPHICS_D2D_D2D_OFFSCREEN_SURFACE_H

#include <vector>

#include <wrl/client.h>
#include <d2d1_1.h>

#include <karin/common/geometry/size.h>
#include "d2d_surface.h"
#include "d2d_consts.h"

namespace karin
{
class D2DOffscreenSurface : public ID2DSurface
{
public:
    D2DOffscreenSurface(Size size);
    ~D2DOffscreenSurface() override = default;

    void cleanUp() override;
    void present() override;
    void resize(Size size) override;

    Microsoft::WRL::ComPtr<ID2D1Bitmap> getTargetBitmap() const override;
    void setDeviceContext(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext) override;

    std::vector<std::byte> getImageData() const;

private:
    void createBitmap();

    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_bitmap;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;

    Size m_size;

    const D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        DEFAULT_DPI, DEFAULT_DPI
    );
};
} // karin

#endif //CORE_SRC_GRAPHICS_D2D_D2D_OFFSCREEN_SURFACE_H
