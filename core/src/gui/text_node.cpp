#include <karin/gui/text_node.h>

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
    if (m_text != text)
    {
        m_text = text;
        requestRelayout();

        YGNodeMarkDirty(m_yogaNode);
    }
}

void TextNode::drawInternal(GraphicsContext& gc)
{
    Rectangle layout = getLayout();
    Point start = layout.pos;

    auto& textEngine = getAppContext().textEngine;
    auto textBlob = textEngine->layoutText(m_text, m_textStyle, m_paragraphStyle, layout.size);

    if (m_drawCaret)
    {

        auto caretPos = calcCaretPosition(textBlob);
        float caretX = caretPos.start.x - m_scrollOffset;

        if (textBlob.layoutSize.width <= layout.size.width)
        {
            m_scrollOffset = 0.0f;
        }
        else
        {
            // adjust caret to left bound
            if (caretX < 0)
            {
                m_scrollOffset = caretPos.start.x - CARET_WIDTH;
            }
            // adjust caret to right bound
            else if (caretX > layout.size.width)
            {
                m_scrollOffset += caretX - layout.size.width;
            }

            m_scrollOffset = std::clamp(m_scrollOffset, 0.0f, textBlob.layoutSize.width - layout.size.width + CARET_WIDTH);
        }

        start.x -= m_scrollOffset;
        gc.drawText(textBlob, start, m_pattern);

        caretPos.start.x -= m_scrollOffset;
        caretPos.end.x -= m_scrollOffset;
        gc.drawLine(caretPos.start, caretPos.end, m_caretPattern, StrokeStyle{.width = CARET_WIDTH});
    }
    else
    {
        start.x -= m_scrollOffset;
        gc.drawText(textBlob, start, m_pattern);
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

void TextNode::setDrawOffsetCharIndex(CalculateOrigin origin, uint32_t index)
{
    m_origin = origin;
    m_offsetIndex = index;
}

TextNode::CaretPosition TextNode::calcCaretPosition(const TextBlob& blob) const
{
    if (m_caretIndex < 0 || m_caretIndex > blob.glyphs.size())
    {
        return {true};
    }

    const FontMetrics metrics = blob.fontFace->getFontMetrics();
    const float scale = blob.fontEmSize / static_cast<float>(metrics.unitsPerEm);

    if (m_caretIndex == blob.glyphs.size())
    {
        Point baseLeft;
        if (blob.glyphs.size() == 0)
        {
            Rectangle layout = getLayout();
            baseLeft = Point(layout.pos.x, layout.pos.y + blob.layoutSize.height);
        }
        else
        {
            const GlyphInfo glyph = blob.glyphs[blob.glyphs.size() - 1];
            baseLeft = Point(glyph.position.x + glyph.advanceX + CARET_WIDTH / 2, glyph.position.y);
        }

        const Point top = Point(baseLeft.x, baseLeft.y - static_cast<float>(metrics.ascender) * scale);
        const Point bottom = Point(baseLeft.x, baseLeft.y + static_cast<float>(metrics.descender) * scale);

        return {false, top, bottom};
    }
    else if (m_caretIndex == 0)
    {
        const GlyphInfo glyph = blob.glyphs[0];

        const float x = glyph.position.x + CARET_WIDTH / 2;
        const Point top = Point(x, glyph.position.y - static_cast<float>(metrics.ascender) * scale);
        const Point bottom = Point(x, glyph.position.y + static_cast<float>(metrics.descender) * scale);

        return {false, top, bottom};
    }
    else
    {
        const GlyphInfo glyph = blob.glyphs[m_caretIndex];

        const float x = glyph.position.x - CARET_WIDTH / 2;
        const Point top = Point(x, glyph.position.y - static_cast<float>(metrics.ascender) * scale);
        const Point bottom = Point(x, glyph.position.y + static_cast<float>(metrics.descender) * scale);

        return {false, top, bottom};
    }
}
} // karin::gui