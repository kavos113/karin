#include <karin/common/geometry/transform2d.h>

#include <cmath>

#include <algorithm>
#include <iostream>
#include <numbers>

#include <glm/glm.hpp>

namespace karin
{
inline glm::mat3& toMat(float *data)
{
    return *reinterpret_cast<glm::mat3*>(data);
}

inline const glm::mat3& toMat(const float *data)
{
    return *reinterpret_cast<const glm::mat3*>(data);
}

Transform2D::Transform2D()
    : m_data()
{
    toMat(m_data) = glm::mat3(1.0f);
}

Transform2D::~Transform2D() = default;

Transform2D::Transform2D(const Transform2D& other)
    : m_data()
{
    toMat(m_data) = toMat(other.m_data);
}

Transform2D& Transform2D::operator=(const Transform2D& other)
{
    toMat(m_data) = toMat(other.m_data);
    return *this;
}

Transform2D& Transform2D::translate(float tx, float ty)
{
    glm::mat3& mat = toMat(m_data);

    glm::mat3 trans(1.0f);
    trans[2] = glm::vec3(tx, ty, 1.0f);
    mat = mat * trans;
    return *this;
}

Transform2D& Transform2D::rotate(float radian)
{
    glm::mat3& mat = toMat(m_data);

    float c = std::cos(radian);
    float s = std::sin(radian);

    glm::mat3 rot(1.0f);
    rot[0][0] = c;
    rot[0][1] = s;
    rot[1][0] = -s;
    rot[1][1] = c;
    mat = mat * rot;

    return *this;
}

Transform2D& Transform2D::rotateDeg(float degree)
{
    glm::mat3& mat = toMat(m_data);

    float radian = degree * std::numbers::pi_v<float> / 180.0f;
    float c = std::cos(radian);
    float s = std::sin(radian);

    glm::mat3 rot(1.0f);
    rot[0][0] = c;
    rot[0][1] = s;
    rot[1][0] = -s;
    rot[1][1] = c;
    mat = mat * rot;

    return *this;
}

Transform2D& Transform2D::scale(float sx, float sy)
{
    glm::mat3& mat = toMat(m_data);

    glm::mat3 scaleMat(1.0f);
    scaleMat[0][0] = sx;
    scaleMat[1][1] = sy;
    mat = mat * scaleMat;

    return *this;
}

Transform2D& Transform2D::multiply(const Transform2D& other)
{
    glm::mat3& mat = toMat(m_data);
    mat = mat * toMat(other.m_data);
    return *this;
}

Transform2D& Transform2D::operator*=(const Transform2D& other)
{
    return multiply(other);
}

Transform2D Transform2D::operator*(const Transform2D& other) const
{
    Transform2D result(*this);
    result.multiply(other);
    return result;
}

std::optional<Transform2D> Transform2D::inverse() const
{
    Transform2D result;
    glm::mat3 invMat = glm::inverse(toMat(m_data));
    if (glm::determinant(toMat(m_data)) == 0.0f)
    {
        return std::nullopt; // Not invertible
    }
    toMat(result.m_data) = invMat;
    return result;
}

const float* Transform2D::data() const
{
    return m_data;
}

std::ostream& operator<<(std::ostream& os, const Transform2D& transform)
{
    const glm::mat3& mat = toMat(transform.data());
    os << "Transform2D(\n";
    for (int i = 0; i < 3; ++i)
    {
        os << "  " << mat[i][0] << ", " << mat[i][1] << ", " << mat[i][2] << "\n";
    }
    os << ")";
    return os;
}

Point operator*(const Transform2D& transform, const Point& point)
{
    const glm::mat3& mat = toMat(transform.data());
    glm::vec3 transformed = mat * glm::vec3(point.x, point.y, 1.0f);
    return Point(transformed.x, transformed.y);
}

Rectangle applyTransform(const Transform2D& transform, const Rectangle& rect)
{
    Point topLeft = transform * rect.pos;
    Point topRight = transform * Point(rect.pos.x + rect.size.width, rect.pos.y);
    Point bottomLeft = transform * Point(rect.pos.x, rect.pos.y + rect.size.height);
    Point bottomRight = transform * Point(rect.pos.x + rect.size.width, rect.pos.y + rect.size.height);

    float minX = std::min({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
    float maxX = std::max({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
    float minY = std::min({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
    float maxY = std::max({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});

    return Rectangle(Point(minX, minY), Size(maxX - minX, maxY - minY));
}
}
