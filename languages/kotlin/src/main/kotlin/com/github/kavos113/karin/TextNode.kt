package com.github.kavos113.karin

import com.github.kavos113.karin.engine.handle.TextNodeHandle

class TextNode(
    text: String = "",
    style: TextStyle = TextStyle(),
    paragraphStyle: ParagraphStyle = ParagraphStyle(),
    color: Color = Color.Black
) : ViewNode(
    TextNodeHandle(text, style, paragraphStyle, color)
) {

}