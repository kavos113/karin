#ifndef GRAPHICS_TEXT_FONT_MANAGER_H
#define GRAPHICS_TEXT_FONT_MANAGER_H

#include <memory>
#include <unordered_map>

#include <karin/graphics/font.h>
#include <karin/graphics/font_face.h>
#include "font_loader.h"

namespace karin
{
class FontManager
{
public:
    FontManager();
    ~FontManager();

    std::shared_ptr<IFontFace> getFontFace(const Font& font);

    void registerCustomFont(const std::string& filePath, const Font& font);

private:
    std::unordered_map<size_t, std::shared_ptr<IFontFace>> m_fontCache;

    std::unique_ptr<FontLoader> m_fontLoader;
};
} // karin

#endif //GRAPHICS_TEXT_FONT_MANAGER_H
