#ifndef GRAPHICS_TEXT_HARFBUZZ_PROVIDER_H
#define GRAPHICS_TEXT_HARFBUZZ_PROVIDER_H

#include <hb.h>

namespace karin
{
class IHarfBuzzProvider
{
public:
    virtual ~IHarfBuzzProvider() = default;

    virtual hb_font_t* getHbFont() const = 0;
};
}

#endif //GRAPHICS_TEXT_HARFBUZZ_PROVIDER_H
