package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Point
import com.github.kavos113.karin.ui.event.KeyEvent

internal interface EventManager {
    fun enablePointerMoveHandler()
    fun enablePointerDownHandler()
    fun enablePointerUpHandler()
    fun enablePointerEnterHandler()
    fun enablePointerLeaveHandler()
    fun enableMouseWheelHandler()
    fun enableKeyHandler()
    fun enableKeyTypeHandler()

    fun setOnClick(handler: () -> Unit)
    fun setOnPointerMove(handler: (Point) -> Unit)
    fun setOnPointerDown(handler: (Point) -> Unit)
    fun setOnPointerUp(handler: (Point) -> Unit)
    fun setOnPointerEnter(handler: () -> Unit)
    fun setOnPointerLeave(handler: () -> Unit)
    fun setOnMouseWheel(handler: (Int) -> Unit)
    fun setOnKeyDown(handler: (KeyEvent) -> Unit)
    fun setOnKeyUp(handler: (KeyEvent) -> Unit)
    fun setOnKeyType(handler: (String) -> Unit)

    val isHovered: State<Boolean>
    val isPressed: State<Boolean>
    val isFocused: State<Boolean>
    fun applyEventState()

    fun clearOnClick()
    fun clearOnPointerMove()
    fun clearOnPointerDown()
    fun clearOnPointerUp()
    fun clearOnPointerEnter()
    fun clearOnPointerLeave()
    fun clearOnMouseWheel()
    fun clearOnKeyDown()
    fun clearOnKeyUp()
    fun clearOnKeyType()

    fun setIsFocusable(isFocusable: Boolean)
}