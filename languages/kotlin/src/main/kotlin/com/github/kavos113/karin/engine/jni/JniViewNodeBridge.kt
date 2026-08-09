package com.github.kavos113.karin.engine.jni

import com.github.kavos113.karin.engine.handle.ViewNodeHandle

internal object JniViewNodeBridge {
    fun setSize(nodePtr: Long, width: Float, height: Float) = JniViewNode.setSize(nodePtr, width, height)
    fun setWidth(nodePtr: Long, width: Float) = JniViewNode.setWidth(nodePtr, width)
    fun setHeight(nodePtr: Long, height: Float) = JniViewNode.setHeight(nodePtr, height)
    fun setBackgroundColor(nodePtr: Long, r: Float, g: Float, b: Float, a: Float) = JniViewNode.setBackgroundColor(nodePtr, r, g, b, a)
    fun setOpacity(nodePtr: Long, opacity: Float) = JniViewNode.setOpacity(nodePtr, opacity)

    const val SIDE_FLAG_LEFT = 0b0001
    const val SIDE_FLAG_TOP = 0b0010
    const val SIDE_FLAG_RIGHT = 0b0100
    const val SIDE_FLAG_BOTTOM = 0b1000

    fun setMargin(nodePtr: Long, flags: Char, left: Float, top: Float, right: Float, bottom: Float) = JniViewNode.setMargin(nodePtr, flags, left, top, right, bottom)
    fun setPadding(nodePtr: Long, flags: Char, left: Float, top: Float, right: Float, bottom: Float) = JniViewNode.setPadding(nodePtr, flags, left, top, right, bottom)
    fun setMarginSide(nodePtr: Long, side: Int, value: Float) = JniViewNode.setMarginSide(nodePtr, side, value)
    fun setPaddingSide(nodePtr: Long, side: Int, value: Float) = JniViewNode.setPaddingSide(nodePtr, side, value)

    fun setBorder(nodePtr: Long, side: Int, width: Float, r: Float, g: Float, b: Float, a: Float, style: Int) = JniViewNode.setBorder(nodePtr, side, width, r, g, b, a, style)
    fun setShadow(nodePtr: Long, offsetX: Float, offsetY: Float, r: Float, g: Float, b: Float, a: Float, blurRadius: Float, spreadRadius: Float) = JniViewNode.setShadow(nodePtr, offsetX, offsetY, r, g, b, a, blurRadius, spreadRadius)

    fun requestRelayout(nodePtr: Long) = JniViewNode.requestRelayout(nodePtr)
    fun requestRedraw(nodePtr: Long) = JniViewNode.requestRedraw(nodePtr)

    fun setPointerMoveHandler(nodePtr: Long, target: ViewNodeHandle) = JniViewNode.setPointerMoveListener(nodePtr, target)
    fun setPointerDownHandler(nodePtr: Long, target: ViewNodeHandle) = JniViewNode.setPointerDownListener(nodePtr, target)
    fun setPointerUpHandler(nodePtr: Long, target: ViewNodeHandle) = JniViewNode.setPointerUpListener(nodePtr, target)
    fun setPointerEnterHandler(nodePtr: Long, target: ViewNodeHandle) = JniViewNode.setPointerEnterListener(nodePtr, target)
    fun setPointerLeaveHandler(nodePtr: Long, target: ViewNodeHandle) = JniViewNode.setPointerLeaveListener(nodePtr, target)
    fun setMouseWheelHandler(nodePtr: Long, target: ViewNodeHandle) = JniViewNode.setMouseWheelListener(nodePtr, target)
    fun setKeyListener(nodePtr: Long, target: ViewNodeHandle) = JniViewNode.setKeyListener(nodePtr, target)
    fun setKeyTypeListener(nodePtr: Long, target: ViewNodeHandle) = JniViewNode.setKeyTypeListener(nodePtr, target)
    fun setIsFocusable(nodePtr: Long, isFocusable: Boolean) = JniViewNode.setIsFocusable(nodePtr, isFocusable)

    fun destroy(nodePtr: Long) = JniViewNode.destroy(nodePtr)
}