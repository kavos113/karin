package com.github.kavos113.karin.engine.jni

internal object JniTextNodeBridge {
    fun create(
        text: String,
        fontFamily: String,
        fontStyle: Int,
        fontStretch: Int,
        fontWeight: Int,
        fontSize: Float,
        locale: String,
        underline: Boolean,
        strikethrough: Boolean,
        horizontalAlignment: Int,
        verticalAlignment: Int,
        lineSpacing: Float,
        baseline: Float,
        lineSpacingMode: Int,
        trimming: Int,
        wrapping: Int,
        flowDirection: Int,
        readingDirection: Int,
        r: Float,
        g: Float,
        b: Float,
        a: Float
    ) = JniTextNode.create(
        text,
        fontFamily,
        fontStyle,
        fontStretch,
        fontWeight,
        fontSize,
        locale,
        underline,
        strikethrough,
        horizontalAlignment,
        verticalAlignment,
        lineSpacing,
        baseline,
        lineSpacingMode,
        trimming,
        wrapping,
        flowDirection,
        readingDirection,
        r,
        g,
        b,
        a
    )

    fun setText(nodePtr: Long, text: String) = JniTextNode.setText(nodePtr, text)

    fun setEnableCaret(nodePtr: Long, enableCaret: Boolean) = JniTextNode.setEnableCaret(nodePtr, enableCaret)
    fun setCaretIndex(nodePtr: Long, caretIndex: Int) = JniTextNode.setCaretIndex(nodePtr, caretIndex)
}