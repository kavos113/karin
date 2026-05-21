package com.github.kavos113.karin

import com.github.kavos113.karin.engine.handle.TextNodeHandle
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextStyle

class TextNode(
    text: String = "",
    style: TextStyle = TextStyle(),
    paragraphStyle: ParagraphStyle = ParagraphStyle(),
    color: Color = Color.Black
) : ViewNode(
    TextNodeHandle(text, style, paragraphStyle, color)
) {

}