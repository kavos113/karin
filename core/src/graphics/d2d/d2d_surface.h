#ifndef CORE_SRC_GRAPHICS_D2D_D2D_SURFACE_H
#define CORE_SRC_GRAPHICS_D2D_D2D_SURFACE_H

#include <d2d1.h>
#include <wrl/client.h>

#include <karin/common/geometry/size.h>

namespace karin
{
class ID2DSurface
{
public:
    virtual ~ID2DSurface() = default;

    virtual void cleanUp() = 0;
    virtual void present() = 0;
    virtual void resize(Size size) = 0;

    virtual Microsoft::WRL::ComPtr<ID2D1Bitmap> getTargetBitmap(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext) const = 0;
};
} // karin

#endif //CORE_SRC_GRAPHICS_D2D_D2D_SURFACE_H
