#ifndef GRAPHICS_D2D_D2D_COLOR_H
#define GRAPHICS_D2D_D2D_COLOR_H

#include <d2d1_1.h>
#include <karin/common/color/color.h>

namespace karin
{
inline D2D1_COLOR_F toD2DColor(const Color& color)
{
    return D2D1::ColorF(color.r, color.g, color.b, color.a);
}
}

#endif //GRAPHICS_D2D_D2D_COLOR_H
