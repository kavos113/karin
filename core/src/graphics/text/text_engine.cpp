#include <karin/graphics/text_engine.h>

#include "text_engine_impl.h"

namespace karin
{
TextEngine::TextEngine()
{
    m_impl = std::make_unique<TextEngineImpl>();
}

TextEngine::~TextEngine() = default;

TextBlob TextEngine::layoutText(
    const std::string& text,
    const TextStyle& textStyle,
    const ParagraphStyle& paragraphStyle,
    const Size& maxSize
) const
{
    return m_impl->layoutText(text, textStyle, paragraphStyle, maxSize);
}

void TextEngine::registerCustomFont(const std::string& filePath, const Font& font) const
{
    m_impl->registerCustomFont(filePath, font);
}
}
