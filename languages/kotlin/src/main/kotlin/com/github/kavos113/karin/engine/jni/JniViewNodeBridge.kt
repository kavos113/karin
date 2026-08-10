package com.github.kavos113.karin.engine.jni

import com.github.kavos113.karin.engine.handle.EventManager

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

    fun setIsFocusable(nodePtr: Long, isFocusable: Boolean) = JniViewNode.setIsFocusable(nodePtr, isFocusable)
    fun setPointerMoveHandler(nodePtr: Long, target: EventManager) = JniViewNode.setPointerMoveListener(nodePtr, target)
    fun setPointerDownHandler(nodePtr: Long, target: EventManager) = JniViewNode.setPointerDownListener(nodePtr, target)
    fun setPointerUpHandler(nodePtr: Long, target: EventManager) = JniViewNode.setPointerUpListener(nodePtr, target)
    fun setPointerEnterHandler(nodePtr: Long, target: EventManager) = JniViewNode.setPointerEnterListener(nodePtr, target)
    fun setPointerLeaveHandler(nodePtr: Long, target: EventManager) = JniViewNode.setPointerLeaveListener(nodePtr, target)
    fun setMouseWheelHandler(nodePtr: Long, target: EventManager) = JniViewNode.setMouseWheelListener(nodePtr, target)
    fun setKeyHandler(nodePtr: Long, target: EventManager) = JniViewNode.setKeyListener(nodePtr, target)
    fun setKeyTypeHandler(nodePtr: Long, target: EventManager) = JniViewNode.setKeyTypeListener(nodePtr, target)
    fun clearPointerMoveHandler(nodePtr: Long) = JniViewNode.clearPointerMoveListener(nodePtr)
    fun clearPointerDownHandler(nodePtr: Long) = JniViewNode.clearPointerDownListener(nodePtr)
    fun clearPointerUpHandler(nodePtr: Long) = JniViewNode.clearPointerUpListener(nodePtr)
    fun clearPointerEnterHandler(nodePtr: Long) = JniViewNode.clearPointerEnterListener(nodePtr)
    fun clearPointerLeaveHandler(nodePtr: Long) = JniViewNode.clearPointerLeaveListener(nodePtr)
    fun clearMouseWheelHandler(nodePtr: Long) = JniViewNode.clearMouseWheelListener(nodePtr)
    fun clearKeyHandler(nodePtr: Long) = JniViewNode.clearKeyListener(nodePtr)
    fun clearKeyTypeHandler(nodePtr: Long) = JniViewNode.clearKeyTypeListener(nodePtr)

    fun destroy(nodePtr: Long) = JniViewNode.destroy(nodePtr)
}