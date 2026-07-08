#ifndef GRAPHICS_OFFSCREEN_RENDERER_TARGET_H
#define GRAPHICS_OFFSCREEN_RENDERER_TARGET_H

#include <cstddef>

#include <vector>

namespace karin
{
class IOffscreenRendererTarget
{
public:
    virtual ~IOffscreenRendererTarget() = default;

    virtual std::vector<std::byte> getImageData() const = 0;
};
} // karin

#endif //GRAPHICS_OFFSCREEN_RENDERER_TARGET_H
