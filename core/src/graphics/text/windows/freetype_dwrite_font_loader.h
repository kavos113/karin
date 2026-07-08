#ifndef GRAPHICS_TEXT_WINDOWS_FREETYPE_DWRITE_FONT_LOADER_H
#define GRAPHICS_TEXT_WINDOWS_FREETYPE_DWRITE_FONT_LOADER_H

#include <cstddef>

#include <string>
#include <vector>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <text/font_loader_platform.h>
#include <karin/graphics/font_face.h>
#include <karin/graphics/font.h>

namespace karin
{
class FreeTypeDWriteFontLoader : public IPlatformFontLoader
{
public:
    FreeTypeDWriteFontLoader();
    ~FreeTypeDWriteFontLoader() override;

    std::unique_ptr<IFontFace> loadFont(const Font& font) override;
    std::unique_ptr<IFontFace> loadFontFromFile(const std::string& filePath) override;
    std::vector<Font> getFontLists() override;

private:
    FT_Library m_library = nullptr;
    std::vector<std::vector<std::byte>> m_fontDataStorage;
};
} // karin

#endif //GRAPHICS_TEXT_WINDOWS_FREETYPE_DWRITE_FONT_LOADER_H
