#include "font_manager.h"

namespace karin
{
FontManager::FontManager()
{
    m_fontLoader = std::make_unique<FontLoader>();
}

std::shared_ptr<IFontFace> FontManager::getFontFace(const Font& font)
{
    if (auto it = m_fontCache.find(font.hash()); it != m_fontCache.end()) {
        return it->second;
    }

    std::shared_ptr<IFontFace> fontFace = m_fontLoader->loadFont(font);
    m_fontCache[font.hash()] = fontFace;

    return fontFace;
}

void FontManager::registerCustomFont(const std::string& filePath, const Font& font)
{
    std::shared_ptr<IFontFace> fontFace = m_fontLoader->loadFontFromFile(filePath);
    m_fontCache[font.hash()] = fontFace;
}
} // karin