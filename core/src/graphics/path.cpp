#include <karin/graphics/path.h>

#include "path_impl.h"

namespace karin
{
Path::Path()
{
    m_impl = std::make_unique<PathImpl>();
}

Path::~Path() = default;

void Path::start(Point start)
{
    detachIfNeeded();
    m_impl->start(start);
}

void Path::lineTo(Point end)
{
    detachIfNeeded();
    m_impl->lineTo(end);
}

void Path::arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle, bool isSmallArc)
{
    detachIfNeeded();
    m_impl->arcTo(center, radiusX, radiusY, startAngle, endAngle, isSmallArc);
}

void Path::close()
{
    detachIfNeeded();
    m_impl->close();
}

void Path::detachIfNeeded()
{
    if (m_impl.use_count() > 1)
    {
        m_impl = std::make_unique<PathImpl>(*m_impl);
    }
}

PathImpl* Path::impl() const
{
    return m_impl.get();
}
} // karin