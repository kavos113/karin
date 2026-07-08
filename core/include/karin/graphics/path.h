#ifndef KARIN_GRAPHICS_PATH_H
#define KARIN_GRAPHICS_PATH_H

#include <memory>
#include <karin/common/geometry/point.h>

namespace karin
{
class PathImpl;

class Path
{
public:
    Path();
    ~Path();

    Path(const Path& other) = default;
    Path& operator=(const Path& other) = default;
    Path(Path&&) noexcept = default;
    Path& operator=(Path&&) noexcept = default;

    void start(Point start);
    void lineTo(Point end);
    void arcTo(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle,
        bool isSmallArc
    );
    void close();

private:
    void detachIfNeeded();

    std::shared_ptr<PathImpl> m_impl;

private:
    friend class Canvas;

    PathImpl* impl() const;
};
} // karin

#endif //KARIN_GRAPHICS_PATH_H
