package com.github.kavos113.karin.engine.jni

import com.github.kavos113.karin.engine.handle.ViewNodeHandle

internal object JniViewNodeBridge {
    fun setClickListener(nodePtr: Long, node: ViewNodeHandle) = JniViewNode.setClickListener(nodePtr, node)
    fun setSize(nodePtr: Long, width: Float, height: Float) = JniViewNode.setSize(nodePtr, width, height)
    fun setWidth(nodePtr: Long, width: Float) = JniViewNode.setWidth(nodePtr, width)
    fun setHeight(nodePtr: Long, height: Float) = JniViewNode.setHeight(nodePtr, height)
    fun setBackgroundColor(nodePtr: Long, r: Float, g: Float, b: Float, a: Float) = JniViewNode.setBackgroundColor(nodePtr, r, g, b, a)
    fun setOpacity(nodePtr: Long, opacity: Float) = JniViewNode.setOpacity(nodePtr, opacity);

    const val MARGIN_FLAG_LEFT = 0b0001
    const val MARGIN_FLAG_TOP = 0b0010
    const val MARGIN_FLAG_RIGHT = 0b0100
    const val MARGIN_FLAG_BOTTOM = 0b1000

    fun setMargin(nodePtr: Long, flags: Char, left: Float, top: Float, right: Float, bottom: Float) = JniViewNode.setMargin(nodePtr, flags, left, top, right, bottom)
    fun setPadding(nodePtr: Long, flags: Char, left: Float, top: Float, right: Float, bottom: Float) = JniViewNode.setPadding(nodePtr, flags, left, top, right, bottom)
    fun setMarginSide(nodePtr: Long, side: Int, value: Float) = JniViewNode.setMarginSide(nodePtr, side, value)
    fun setPaddingSide(nodePtr: Long, side: Int, value: Float) = JniViewNode.setPaddingSide(nodePtr, side, value)

    fun requestRelayout(nodePtr: Long) = JniViewNode.requestRelayout(nodePtr)
    fun requestRedraw(nodePtr: Long) = JniViewNode.requestRedraw(nodePtr)

    fun destroy(nodePtr: Long) = JniViewNode.destroy(nodePtr)
}