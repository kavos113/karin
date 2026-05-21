package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextStyle
import com.github.kavos113.karin.engine.jni.JniTextNodeBridge

internal class TextNodeHandle(ptr: Long) : ViewNodeHandle(ptr) {

    constructor(
        text: String = "",
        style: TextStyle = TextStyle(),
        paragraphStyle: ParagraphStyle = ParagraphStyle(),
        color: Color = Color.Black
    ) : this(
        JniTextNodeBridge.create(
            text,
            style.fontFamily,
            style.fontStyle.toInt(),
            style.fontStretch.toInt(),
            style.fontWeight.toInt(),
            style.fontSize,
            style.locale,
            false, // TODO
            false,
            paragraphStyle.textHorizontalAlignment.toInt(),
            paragraphStyle.textVerticalAlignment.toInt(),
            paragraphStyle.lineSpacingValue,
            paragraphStyle.baseLineOffset,
            paragraphStyle.textLineSpacing.toInt(),
            paragraphStyle.textTrim.toInt(),
            paragraphStyle.textWrap.toInt(),
            paragraphStyle.flowDirection.toInt(),
            paragraphStyle.readingDirection.toInt(),
            color.r, color.g, color.b, color.a
        )
    )
}