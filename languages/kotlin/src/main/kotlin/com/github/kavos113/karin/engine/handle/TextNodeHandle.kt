package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.engine.jni.JniTextNodeBridge
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextStyle

internal class TextNodeHandle(ptr: Long) : ViewNodeHandle(ptr) {

    constructor(
        text: String = "",
        style: TextStyle = TextStyle(),
        paragraphStyle: ParagraphStyle = ParagraphStyle(),
    ) : this(
        JniTextNodeBridge.create(
            text,
            style.fontFamily,
            style.fontStyle.value,
            style.fontStretch.value,
            style.fontWeight.value,
            style.fontSize,
            style.locale,
            false, // TODO
            false,
            paragraphStyle.textAlign.value,
            paragraphStyle.textVerticalAlignment.value,
            paragraphStyle.lineSpacingValue,
            paragraphStyle.baseLineOffset,
            paragraphStyle.textLineSpacing.value,
            paragraphStyle.textTrim.value,
            paragraphStyle.textWrap.value,
            paragraphStyle.flowDirection.value,
            paragraphStyle.readingDirection.value,
            style.color.r,
            style.color.g,
            style.color.b,
            style.color.a
        )
    )

    fun setText(text: String) {
        JniTextNodeBridge.setText(ptr, text)
    }
}