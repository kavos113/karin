#include "text_engine_impl.h"

namespace karin
{
TextEngineImpl::TextEngineImpl()
{
    m_fontManager = std::make_unique<FontManager>();
    m_textLayouter = std::make_unique<TextLayouter>();
}

TextBlob TextEngineImpl::layoutText(
    const std::string& text,
    const TextStyle& textStyle,
    const ParagraphStyle& paragraphStyle,
    const Size& maxSize
) const
{
    std::shared_ptr<IFontFace> fontFace = m_fontManager->getFontFace(textStyle.font);

    Size layoutSize{};
    std::vector<GlyphPosition> glyphs = TextLayouter::layout(
        fontFace.get(),
        text,
        textStyle,
        paragraphStyle,
        maxSize,
        layoutSize
    );

    return TextBlob{
        .glyphs = std::move(glyphs),
        .fontFace = std::move(fontFace),
        .font = textStyle.font,
        .fontEmSize = textStyle.size,
        .layoutSize = layoutSize,
    };
}

void TextEngineImpl::registerCustomFont(const std::string& filePath, const Font& font) const
{
    m_fontManager->registerCustomFont(filePath, font);
}
}
