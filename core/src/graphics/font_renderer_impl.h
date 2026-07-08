#ifndef GRAPHICS_FONT_RENDERER_IMPL_H
#define GRAPHICS_FONT_RENDERER_IMPL_H

#include <karin/common/geometry/point.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/text_blob.h>
#include <karin/graphics/graphics_context.h>

namespace karin
{
class IFontRendererImpl
{
public:
    IFontRendererImpl() = default;
    virtual ~IFontRendererImpl() = default;

    virtual void drawText(const TextBlob& text, Point start, const Pattern& pattern, const GraphicsContext::State& state) const = 0;
};
} // karin

#endif //GRAPHICS_FONT_RENDERER_IMPL_H
