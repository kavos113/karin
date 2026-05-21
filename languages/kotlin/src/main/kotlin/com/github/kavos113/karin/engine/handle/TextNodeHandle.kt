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
            paragraphStyle.horizontalAlignment.toInt(),
            paragraphStyle.verticalAlignment.toInt(),
            paragraphStyle.lineSpacingValue,
            paragraphStyle.baseLineOffset,
            paragraphStyle.lineSpacingMode.toInt(),
            paragraphStyle.trimMode.toInt(),
            paragraphStyle.wrapMode.toInt(),
            paragraphStyle.flowDirection.toInt(),
            paragraphStyle.readingDirection.toInt(),
            color.r, color.g, color.b, color.a
        )
    )
}