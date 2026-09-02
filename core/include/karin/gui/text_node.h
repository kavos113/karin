#ifndef KARIN_GUI_TEXT_NODE_H
#define KARIN_GUI_TEXT_NODE_H

#include <cstdint>

#include <string>

#include <karin/common/geometry/size.h>
#include <karin/common/geometry/point.h>
#include <karin/graphics/text_style.h>
#include <karin/graphics/paragraph_style.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/graphics_context.h>

#include "leaf_node.h"

namespace karin::gui
{
class TextNode : public LeafNode
{
public:
    explicit TextNode(
        std::string text,
        TextStyle textStyle,
        ParagraphStyle paragraphStyle,
        Pattern pattern
    );
    ~TextNode() override = default;

    void setText(const std::string& text);
    void setDrawCaret(bool drawCaret);
    void setEnableScroll(bool enableScroll);

    // draw caret behind text[caretIndex]. 0 <= caretIndex <= text.length
    void setCaretIndex(uint32_t caretIndex);

    void drawInternal(GraphicsContext& gc) override;

protected:
    YGSize measure(Size availableSize) const override;

private:
    struct CaretPosition
    {
        bool isInvalid = false;
        Point start = Point(0, 0);
        Point end = Point(0, 0);
    };
    CaretPosition calcCaretPosition(const TextBlob& blob) const;
    void alignScrollToCaret(const TextBlob& blob, const CaretPosition& caretPos);

    std::string m_text;

    TextStyle m_textStyle;
    ParagraphStyle m_paragraphStyle;
    Pattern m_pattern;

    bool m_drawCaret = false;
    uint32_t m_caretIndex = 0;
    Pattern m_caretPattern;
    bool m_needAlignToCaret = false;

    bool m_enableScroll = false;
    float m_scrollOffset = 0;

    static constexpr float CARET_WIDTH = 1.0f;
    static constexpr float WHEEL_SCROLL_BY_DELTA_UNIT = 10.0f;
};
} // karin::gui

#endif //KARIN_GUI_TEXT_NODE_H
