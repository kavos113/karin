#ifndef GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H
#define GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H

#include <memory>

#include <karin/graphics/text_blob.h>
#include <karin/graphics/text_style.h>
#include <karin/graphics/paragraph_style.h>

#include "font_manager.h"
#include "text_layouter.h"

namespace karin
{
class TextEngineImpl
{
public:
    TextEngineImpl();
    ~TextEngineImpl() = default;

    TextBlob layoutText(
        const std::string& text,
        const TextStyle& textStyle,
        const ParagraphStyle& paragraphStyle,
        const Size& maxSize
    ) const;

    void registerCustomFont(const std::string& filePath, const Font& font) const;

private:
    std::unique_ptr<FontManager> m_fontManager;
    std::unique_ptr<TextLayouter> m_textLayouter;
};
}


#endif //GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H
