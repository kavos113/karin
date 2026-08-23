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
    if (m_text != text)
    {
        m_text = text;
        requestRelayout();

        YGNodeMarkDirty(m_yogaNode);
    }
}

void TextNode::drawInternal(GraphicsContext& gc) const
{
    Rectangle layout = getLayout();
    Point start = layout.pos;

    auto& textEngine = getAppContext().textEngine;
    auto textBlob = textEngine->layoutText(m_text, m_textStyle, m_paragraphStyle, layout.size);

    std::cout << "layout size: " << layout.size << std::endl;

    if (m_origin == CalculateOrigin::Left)
    {
        if (m_offsetIndex < textBlob.glyphs.size())
        {
            const GlyphInfo target = textBlob.glyphs[m_offsetIndex];

            // TODO: multi-line text is not working
            start = Point(target.position.x, start.y);
        }
    }
    else if (m_origin == CalculateOrigin::Right)
    {

    }

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

    std::cout << "measured size: "<< textBlob.layoutSize << std::endl;
    std::cout << "available size: " << availableSize << std::endl;

    Size measuredSize = textBlob.layoutSize;
    if (m_alignToParent)
    {
        return {
            std::min(availableSize.width, measuredSize.width),
            std::min(availableSize.height, measuredSize.height)
        };
    }

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

void TextNode::setAlignToParent(bool alignToParent)
{
    m_alignToParent = alignToParent;
}

void TextNode::drawCaret(GraphicsContext& gc, const TextBlob& blob) const
{
    if (m_caretIndex < 0 || m_caretIndex > blob.glyphs.size())
    {
        return;
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

        gc.drawLine(top, bottom, m_caretPattern, StrokeStyle{.width = CARET_WIDTH});
    }
    else
    {
        const GlyphInfo glyph = blob.glyphs[m_caretIndex];

        const float x = glyph.position.x - CARET_WIDTH / 2;
        const Point top = Point(x, glyph.position.y - static_cast<float>(metrics.ascender) * scale);
        const Point bottom = Point(x, glyph.position.y + static_cast<float>(metrics.descender) * scale);

        gc.drawLine(top, bottom, m_caretPattern, StrokeStyle{.width = CARET_WIDTH});
    }
}
} // karin::gui