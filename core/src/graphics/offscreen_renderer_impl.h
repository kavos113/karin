#ifndef CORE_SRC_GRAPHICS_IOFFSCREENRENDERERIMPL_H
#define CORE_SRC_GRAPHICS_IOFFSCREENRENDERERIMPL_H

#include <vector>

namespace karin
{
class IOffscreenRendererImpl
{
public:
    virtual ~IOffscreenRendererImpl() = default;

    virtual std::vector<std::byte> getImageData() const = 0;
};
} // karin

#endif //CORE_SRC_GRAPHICS_IOFFSCREENRENDERERIMPL_H
