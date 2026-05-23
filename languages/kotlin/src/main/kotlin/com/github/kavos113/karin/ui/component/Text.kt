package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.engine.handle.TextNodeHandle
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.text.FontWeight
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextAlign
import com.github.kavos113.karin.ui.text.TextStyle

fun UiBuilder.Text(
    text: String = "",
    color: Color? = null,
    fontSize: Float? = null,
    fontWeight: FontWeight? = null,
    textAlign: TextAlign? = null,
    style: TextStyle = TextStyle(),
    paragraphStyle: ParagraphStyle = ParagraphStyle(),
) {
    val finalStyle = style.copy(
        color = color ?: style.color,
        fontSize = fontSize ?: style.fontSize,
        fontWeight = fontWeight ?: style.fontWeight
    )
    val finalParagraphStyle = paragraphStyle.copy(
        textAlign = textAlign ?: paragraphStyle.textAlign
    )
    val handle = TextNodeHandle(text, finalStyle, finalParagraphStyle)
    parentContainer.addChild(handle)
}

fun UiBuilder.Text(
    text: State<String>,
    color: Color? = null,
    fontSize: Float? = null,
    fontWeight: FontWeight? = null,
    textAlign: TextAlign? = null,
    style: TextStyle = TextStyle(),
    paragraphStyle: ParagraphStyle = ParagraphStyle(),
) {
    val finalStyle = style.copy(
        color = color ?: style.color,
        fontSize = fontSize ?: style.fontSize,
        fontWeight = fontWeight ?: style.fontWeight
    )
    val finalParagraphStyle = paragraphStyle.copy(
        textAlign = textAlign ?: paragraphStyle.textAlign
    )
    val handle = TextNodeHandle(text.value, finalStyle, finalParagraphStyle)
    parentContainer.addChild(handle)

    text.onChange { newText ->
        handle.setText(newText)
    }
}