package com.github.kavos113.karin.engine.jni

import com.github.kavos113.karin.engine.handle.ViewNodeHandle

internal object JniViewNodeBridge {
    fun setClickListener(nodePtr: Long, node: ViewNodeHandle) = JniViewNode.setClickListener(nodePtr, node)
    fun setSize(nodePtr: Long, width: Float, height: Float) = JniViewNode.setSize(nodePtr, width, height)
    fun setWidth(nodePtr: Long, width: Float) = JniViewNode.setWidth(nodePtr, width)
    fun setHeight(nodePtr: Long, height: Float) = JniViewNode.setHeight(nodePtr, height)
    fun setBackgroundColor(nodePtr: Long, r: Float, g: Float, b: Float, a: Float) = JniViewNode.setBackgroundColor(nodePtr, r, g, b, a)
    fun setOpacity(nodePtr: Long, opacity: Float) = JniViewNode.setOpacity(nodePtr, opacity);

    const val SIDE_FLAG_LEFT = 0b0001
    const val SIDE_FLAG_TOP = 0b0010
    const val SIDE_FLAG_RIGHT = 0b0100
    const val SIDE_FLAG_BOTTOM = 0b1000

    fun setMargin(nodePtr: Long, flags: Char, left: Float, top: Float, right: Float, bottom: Float) = JniViewNode.setMargin(nodePtr, flags, left, top, right, bottom)
    fun setPadding(nodePtr: Long, flags: Char, left: Float, top: Float, right: Float, bottom: Float) = JniViewNode.setPadding(nodePtr, flags, left, top, right, bottom)
    fun setMarginSide(nodePtr: Long, side: Int, value: Float) = JniViewNode.setMarginSide(nodePtr, side, value)
    fun setPaddingSide(nodePtr: Long, side: Int, value: Float) = JniViewNode.setPaddingSide(nodePtr, side, value)

    fun setBorder(nodePtr: Long, side: Int, width: Float, r: Float, g: Float, b: Float, a: Float, style: Int) = JniViewNode.setBorder(nodePtr, side, width, r, g, b, a, style)
    fun setBorderTop(nodePtr: Long, width: Float, r: Float, g: Float, b: Float, a: Float, style: Int) = JniViewNode.setBorderTop(nodePtr,  width, r, g, b, a, style)
    fun setBorderBottom(nodePtr: Long, width: Float, r: Float, g: Float, b: Float, a: Float, style: Int) = JniViewNode.setBorderBottom(nodePtr,  width, r, g, b, a, style)
    fun setBorderLeft(nodePtr: Long, width: Float, r: Float, g: Float, b: Float, a: Float, style: Int) = JniViewNode.setBorderLeft(nodePtr,  width, r, g, b, a, style)
    fun setBorderRight(nodePtr: Long, width: Float, r: Float, g: Float, b: Float, a: Float, style: Int) = JniViewNode.setBorderRight(nodePtr,  width, r, g, b, a, style)

    fun requestRelayout(nodePtr: Long) = JniViewNode.requestRelayout(nodePtr)
    fun requestRedraw(nodePtr: Long) = JniViewNode.requestRedraw(nodePtr)

    fun destroy(nodePtr: Long) = JniViewNode.destroy(nodePtr)
}