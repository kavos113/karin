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
    , m_scrollbarPattern(SolidColorPattern(Color(0.6f, 0.6f, 0.6f)))
{
}

void TextNode::setText(const std::string& text)
{
    if (m_text != text)
    {
        m_text = text;
        requestRelayout();

        m_needAlignToCaret = true;
        YGNodeMarkDirty(m_yogaNode);
    }
}

void TextNode::drawInternal(GraphicsContext& gc)
{
    Rectangle layout = getLayout();
    Point start = layout.pos;

    auto& textEngine = getAppContext().textEngine;
    auto textBlob = textEngine->layoutText(m_text, m_textStyle, m_paragraphStyle, layout.size);

    if (!m_enableScroll)
    {
        gc.drawText(textBlob, start, m_pattern);
        return;
    }

    if (m_drawCaret)
    {
        auto caretPos = calcCaretPosition(textBlob);

        if (m_needAlignToCaret)
        {
            alignScrollToCaret(textBlob, caretPos);
            m_needAlignToCaret = false;
        }

        m_scrollOffset = std::clamp(m_scrollOffset, 0.0f, textBlob.layoutSize.width - layout.size.width + CARET_WIDTH);

        start.x -= m_scrollOffset;
        gc.drawText(textBlob, start, m_pattern);

        caretPos.start.x -= m_scrollOffset;
        caretPos.end.x -= m_scrollOffset;
        gc.drawLine(caretPos.start, caretPos.end, m_caretPattern, StrokeStyle{.width = CARET_WIDTH});
    }
    else
    {
        m_scrollOffset = std::clamp(m_scrollOffset, 0.0f, textBlob.layoutSize.width - layout.size.width + CARET_WIDTH);

        start.x -= m_scrollOffset;
        gc.drawText(textBlob, start, m_pattern);
    }

    if (m_drawScrollBar)
    {
        if (!calcScrollBarPosition(textBlob))
        {
            return;
        }

        gc.drawLine(
            m_scrollBarStart, m_scrollBarEnd,
            m_scrollbarPattern,
            StrokeStyle{.width = SCROLLBAR_WIDTH, .start_cap_style = StrokeStyle::CapStyle::Round, .end_cap_style = StrokeStyle::CapStyle::Round}
        );
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

void TextNode::setEnableScroll(bool enableScroll)
{
    if (enableScroll == m_enableScroll)
    {
        return;
    }

    m_enableScroll = enableScroll;

    if (enableScroll)
    {
        setMouseWheelHandler([this](Point, int delta)
        {
            Application::sendTaskEvent([this, delta]
            {
                m_scrollOffset -= static_cast<float>(delta) * WHEEL_SCROLL_BY_DELTA_UNIT / MouseWheelEvent::DELTA_UNIT;
                if (m_scrollOffset < 0)
                {
                    m_scrollOffset = 0;
                }
                requestRedraw();
            });
        });
    }
    else
    {
        setMouseWheelHandler(nullptr);
    }
}

void TextNode::setCaretIndex(uint32_t caretIndex)
{
    if (caretIndex != m_caretIndex)
    {
        m_caretIndex = caretIndex;
        m_needAlignToCaret = true;
    }
}

void TextNode::setDrawScrollBar(bool drawScrollBar)
{
    if (drawScrollBar == m_drawScrollBar)
    {
        return;
    }

    m_drawScrollBar = drawScrollBar;

    if (drawScrollBar)
    {
        setPointerDownHandler([this](Point point, MouseButtonType type)
        {
            if (type != MouseButtonType::Left)
            {
                return;
            }

            Application::sendTaskEvent([this, point]
            {
                if (!hitScrollBar(point))
                {
                    return;
                }

                m_scrollBarPressedPosition = point;
                m_isScrollBarPressed = true;
            });
        });

        setPointerMoveHandler([this](Point point)
        {
            if (!m_isScrollBarPressed)
            {
                return;
            }

            Application::sendTaskEvent([this, point]
            {
                m_scrollOffset += (point.x - m_scrollBarPressedPosition.x) / m_scrollBarScale;
            });
        });

        setPointerUpHandler([this](Point, MouseButtonType type)
        {
            if (type != MouseButtonType::Left)
            {
                return;
            }

            Application::sendTaskEvent([this]
            {
                m_isScrollBarPressed = false;
            });
        });
    }
    else
    {
        setPointerDownHandler(nullptr);
        setPointerMoveHandler(nullptr);
        setPointerUpHandler(nullptr);
    }
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

void TextNode::alignScrollToCaret(const TextBlob& blob, const CaretPosition& caretPos)
{
    Rectangle layout = getLayout();
    float caretX = caretPos.start.x - m_scrollOffset;

    if (blob.layoutSize.width <= layout.size.width)
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
    }
}

bool TextNode::calcScrollBarPosition(const TextBlob& blob)
{
    Rectangle layout = getLayout();

    if (blob.layoutSize.width <= 0.0f || layout.size.width > blob.layoutSize.width )
    {
        return false;
    }

    float scale = layout.size.width / blob.layoutSize.width;
    m_scrollBarStart.x = m_scrollOffset * scale + layout.pos.x;
    m_scrollBarEnd.x = (m_scrollOffset + layout.size.width) * scale + layout.pos.x;

    float scrollBarY = layout.pos.y + layout.size.height + SCROLLBAR_WIDTH / 2.0f;
    m_scrollBarStart.y = m_scrollBarEnd.y = scrollBarY;
    m_scrollBarScale = scale;

    return true;
}

bool TextNode::hitScrollBar(Point point) const
{
    return (m_scrollBarStart.x <= point.x && point.x <= m_scrollBarEnd.x) && (std::abs(point.y - m_scrollBarStart.y) <= SCROLLBAR_WIDTH / 2.0);
}
} // karin::gui