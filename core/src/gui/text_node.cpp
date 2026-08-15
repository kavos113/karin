#include <karin/gui/text_node.h>

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

    FontMetrics metrics = blob.fontFace->getFontMetrics();
    float scale = blob.fontEmSize / static_cast<float>(metrics.unitsPerEm);

    if (m_caretIndex == blob.glyphs.size())
    {
        GlyphInfo glyph = blob.glyphs[blob.glyphs.size() - 1];

        float x = glyph.position.x + glyph.advanceX;
        Point top = Point(x, glyph.position.y - static_cast<float>(metrics.capHeight) * scale);
        Point bottom = Point(x, glyph.position.y);

        gc.drawLine(top, bottom, m_caretPattern, StrokeStyle{.width = CARET_WIDTH});
    }
    else
    {
        GlyphInfo glyph = blob.glyphs[m_caretIndex];

        float x = glyph.position.x - CARET_WIDTH / 2;
        Point top = Point(x, glyph.position.y - static_cast<float>(metrics.capHeight) * scale);
        Point bottom = Point(x, glyph.position.y);

        std::cout << "caret top: " << top << ", bottom: " << bottom << std::endl;

        gc.drawLine(top, bottom, m_caretPattern, StrokeStyle{.width = CARET_WIDTH});
    }
}
} // karin::gui