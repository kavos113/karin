#include <karin/gui/text_node.h>

#include <ranges>
#include <algorithm>

#include "application_context.h"

namespace karin::gui
{
TextNode::TextNode(std::string text, TextStyle textStyle, ParagraphStyle paragraphStyle, Pattern pattern)
    : m_text(std::move(text))
    , m_textStyle(std::move(textStyle))
    , m_paragraphStyle(paragraphStyle)
    , m_pattern(std::move(pattern))
    , m_caretPattern(SolidColorPattern(Color(Color::Black)))
{
}

void TextNode::setText(const std::string& text)
{
    m_text = text;
    requestRelayout();
}

void TextNode::drawInternal(GraphicsContext& gc) const
{
    Rectangle layout = getLayout();
    Point start = layout.pos;

    auto& textEngine = getAppContext().textEngine;
    auto textBlob = textEngine->layoutText(m_text, m_textStyle, m_paragraphStyle, layout.size);

    gc.drawText(textBlob, start, m_pattern);

    if (m_drawCaret)
    {
        drawCaret(gc, textBlob);
    }
}

YGSize TextNode::measure(Size availableSize) const
{
    auto& textEngine = getAppContext().textEngine;
    auto textBlob = textEngine->layoutText(m_text, m_textStyle, m_paragraphStyle, availableSize);

    Size measuredSize = textBlob.layoutSize;
    return YGSize{measuredSize.width, measuredSize.height};
}

void TextNode::setDrawCaret(bool drawCaret)
{
    m_drawCaret = drawCaret;
}

void TextNode::setCaretIndex(uint32_t caretIndex)
{
    m_caretIndex = caretIndex;
}

void TextNode::drawCaret(GraphicsContext& gc, const TextBlob& blob) const
{
    if (m_caretIndex < 0 || m_caretIndex > blob.glyphs.size())
    {
        return;
    }

    const FontMetrics metrics = blob.fontFace->getFontMetrics();
    const float scale = blob.fontEmSize / static_cast<float>(metrics.unitsPerEm);
    const float biggestY = std::ranges::max(blob.glyphs | std::views::transform(&GlyphInfo::advanceX));

    if (m_caretIndex == blob.glyphs.size())
    {
        const GlyphInfo glyph = blob.glyphs[blob.glyphs.size() - 1];

        const float x = glyph.position.x + glyph.advanceX;
        const Point top = Point(x, biggestY - static_cast<float>(metrics.capHeight) * scale);
        const Point bottom = Point(x, biggestY);

        gc.drawLine(top, bottom, m_caretPattern, StrokeStyle{.width = CARET_WIDTH});
    }
    else
    {
        const GlyphInfo glyph = blob.glyphs[m_caretIndex];

        const float x = glyph.position.x - CARET_WIDTH / 2;
        const Point top = Point(x, biggestY - static_cast<float>(metrics.capHeight) * scale);
        const Point bottom = Point(x, biggestY);

        std::cout << "caret top: " << top << ", bottom: " << bottom << std::endl;

        gc.drawLine(top, bottom, m_caretPattern, StrokeStyle{.width = CARET_WIDTH});
    }
}
} // karin::gui