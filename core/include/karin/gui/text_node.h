#ifndef KARIN_GUI_TEXT_NODE_H
#define KARIN_GUI_TEXT_NODE_H

#include <karin/graphics/text_style.h>
#include <karin/graphics/paragraph_style.h>
#include <karin/graphics/pattern.h>

#include "leaf_node.h"

#include <string>

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

    // draw caret behind text[caretIndex]. 0 <= caretIndex <= text.length
    void setCaretIndex(uint32_t caretIndex);

    void drawInternal(GraphicsContext& gc) const override;
    YGSize measure(Size availableSize) const override;

private:
    void drawCaret(GraphicsContext& gc, const TextBlob& blob) const;

    std::string m_text;

    TextStyle m_textStyle;
    ParagraphStyle m_paragraphStyle;
    Pattern m_pattern;

    bool m_drawCaret = false;
    uint32_t m_caretIndex = 0;
    Pattern m_caretPattern;

    static constexpr float CARET_WIDTH = 2.0f;
};
} // karin::gui

#endif //KARIN_GUI_TEXT_NODE_H
