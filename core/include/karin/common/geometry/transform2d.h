#ifndef KARIN_COMMON_GEOMETRY_TRANSFORM2D_H
#define KARIN_COMMON_GEOMETRY_TRANSFORM2D_H

#include <memory>
#include <optional>

#include "point.h"
#include "rectangle.h"

namespace karin
{
class Transform2D
{
public:
    Transform2D();
    ~Transform2D();
    Transform2D(const Transform2D& other);
    Transform2D& operator=(const Transform2D& other);

    Transform2D& translate(float tx, float ty);
    Transform2D& rotate(float radian);
    Transform2D& rotateDeg(float degree);
    Transform2D& scale(float sx, float sy);

    Transform2D& multiply(const Transform2D& other);
    Transform2D& operator*=(const Transform2D& other);
    Transform2D operator*(const Transform2D& other) const;

    std::optional<Transform2D> inverse() const;

    const float *data() const;

private:
    float m_data[9];
};

std::ostream& operator<<(std::ostream& os, const Transform2D& transform);

Point operator*(const Transform2D& transform, const Point& point);

// create transformed AABB
Rectangle applyTransform(const Transform2D& transform, const Rectangle& rect);

} // karin

#endif //KARIN_COMMON_GEOMETRY_TRANSFORM2D_H
