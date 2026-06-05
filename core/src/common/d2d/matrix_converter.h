#ifndef SRC_COMMON_D2D_MATRIX_CONVERTER_H
#define SRC_COMMON_D2D_MATRIX_CONVERTER_H

#include <d2d1.h>
#include <karin/common/geometry/transform2d.h>

namespace karin
{

inline D2D1_MATRIX_3X2_F toD2DMatrix(const Transform2D& transform)
{
    const float* data = transform.data();

    return D2D1::Matrix3x2F(
        data[0], data[1],
        data[3], data[4],
        data[6], data[7]
    );
}
}


#endif //SRC_COMMON_D2D_MATRIX_CONVERTER_H