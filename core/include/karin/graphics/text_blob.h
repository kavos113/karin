#ifndef KARIN_GRAPHICS_TEXT_BLOB_H
#define KARIN_GRAPHICS_TEXT_BLOB_H

#include <vector>
#include <memory>

#include "font_face.h"
#include "font.h"
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/size.h>

namespace karin
{

struct GlyphInfo
{
    // position in layout. pixels
    // bottom-left of the glyph bounding box
    Point position;

    float advanceX;
    uint32_t glyphIndex;
};

struct TextBlob
{
    std::vector<GlyphInfo> glyphs;
    std::shared_ptr<IFontFace> fontFace;
    Font font;
    float fontEmSize;

    // actual layout size. may be smaller than specified layout size
    Size layoutSize;
};

}

#endif //KARIN_GRAPHICS_TEXT_BLOB_H