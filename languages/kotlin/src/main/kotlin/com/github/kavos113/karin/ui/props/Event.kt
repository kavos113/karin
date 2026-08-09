package com.github.kavos113.karin.ui.props

import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Point
import com.github.kavos113.karin.ui.event.KeyEvent

@ConsistentCopyVisibility
data class Event private constructor(
    val onClick: (() -> Unit)? = null,
    val onClickState: State<(() -> Unit)?>? = null,

    val onPointerMove: ((Point) -> Unit)? = null,
    val onPointerMoveState: State<((Point) -> Unit)?>? = null,
    val onPointerDown: ((Point) -> Unit)? = null,
    val onPointerDownState: State<((Point) -> Unit)?>? = null,
    val onPointerUp: ((Point) -> Unit)? = null,
    val onPointerUpState: State<((Point) -> Unit)?>? = null,
    val onPointerEnter: (() -> Unit)? = null,
    val onPointerEnterState: State<(() -> Unit)?>? = null,
    val onPointerLeave: (() -> Unit)? = null,
    val onPointerLeaveState: State<(() -> Unit)?>? = null,
    val onMouseWheel: ((Int) -> Unit)? = null,
    val onMouseWheelState: State<((Int) -> Unit)?>? = null,

    val onKeyDown: ((KeyEvent) -> Unit)? = null,
    val onKeyDownState: State<((KeyEvent) -> Unit)?>? = null,
    val onKeyUp: ((KeyEvent) -> Unit)? = null,
    val onKeyUpState: State<((KeyEvent) -> Unit)?>? = null,
    val onKeyType: ((String) -> Unit)? = null,
    val onKeyTypeState: State<((String) -> Unit)?>? = null,
) {
    fun onClick(handler: () -> Unit) = copy(onClick = handler)
    fun onClick(state: State<(() -> Unit)?>) = copy(onClickState = state)

    fun onPointerMove(handler: (Point) -> Unit) = copy(onPointerMove = handler)
    fun onPointerMove(state: State<((Point) -> Unit)?>) = copy(onPointerMoveState = state)
    fun onPointerDown(handler: (Point) -> Unit) = copy(onPointerDown = handler)
    fun onPointerDown(state: State<((Point) -> Unit)?>) = copy(onPointerDownState = state)
    fun onPointerUp(handler: (Point) -> Unit) = copy(onPointerUp = handler)
    fun onPointerUp(state: State<((Point) -> Unit)?>) = copy(onPointerUpState = state)
    fun onPointerEnter(handler: () -> Unit) = copy(onPointerEnter = handler)
    fun onPointerEnter(state: State<(() -> Unit)?>) = copy(onPointerEnterState = state)
    fun onPointerLeave(handler: () -> Unit) = copy(onPointerLeave = handler)
    fun onPointerLeave(state: State<(() -> Unit)?>) = copy(onPointerLeaveState = state)
    fun onMouseWheel(handler: (Int) -> Unit) = copy(onMouseWheel = handler)
    fun onMouseWheel(state: State<((Int) -> Unit)?>) = copy(onMouseWheelState = state)

    fun onKeyDown(handler: (KeyEvent) -> Unit) = copy(onKeyDown = handler)
    fun onKeyDown(state: State<((KeyEvent) -> Unit)?>) = copy(onKeyDownState = state)
    fun onKeyUp(handler: (KeyEvent) -> Unit) = copy(onKeyUp = handler)
    fun onKeyUp(state: State<((KeyEvent) -> Unit)?>) = copy(onKeyUpState = state)
    fun onKeyType(handler: (String) -> Unit) = copy(onKeyType = handler)
    fun onKeyType(state: State<((String) -> Unit)?>) = copy(onKeyTypeState = state)

    companion object {
        val Default = Event()

        fun onClick(handler: () -> Unit) = Event().onClick(handler)
        fun onClick(state: State<(() -> Unit)?>) = Event().onClick(state)

        fun onPointerMove(handler: (Point) -> Unit) = Event().onPointerMove(handler)
        fun onPointerMove(state: State<((Point) -> Unit)?>) = Event().onPointerMove(state)
        fun onPointerDown(handler: (Point) -> Unit) = Event().onPointerDown(handler)
        fun onPointerDown(state: State<((Point) -> Unit)?>) = Event().onPointerDown(state)
        fun onPointerUp(handler: (Point) -> Unit) = Event().onPointerUp(handler)
        fun onPointerUp(state: State<((Point) -> Unit)?>) = Event().onPointerUp(state)
        fun onPointerEnter(handler: () -> Unit) = Event().onPointerEnter(handler)
        fun onPointerEnter(state: State<(() -> Unit)?>) = Event().onPointerEnter(state)
        fun onPointerLeave(handler: () -> Unit) = Event().onPointerLeave(handler)
        fun onPointerLeave(state: State<(() -> Unit)?>) = Event().onPointerLeave(state)
        fun onMouseWheel(handler: (Int) -> Unit) = Event().onMouseWheel(handler)
        fun onMouseWheel(state: State<((Int) -> Unit)?>) = Event().onMouseWheel(state)

        fun onKeyDown(handler: (KeyEvent) -> Unit) = Event().onKeyDown(handler)
        fun onKeyDown(state: State<((KeyEvent) -> Unit)?>) = Event().onKeyDown(state)
        fun onKeyUp(handler: (KeyEvent) -> Unit) = Event().onKeyUp(handler)
        fun onKeyUp(state: State<((KeyEvent) -> Unit)?>) = Event().onKeyUp(state)
        fun onKeyType(handler: (String) -> Unit) = Event().onKeyType(handler)
        fun onKeyType(state: State<((String) -> Unit)?>) = Event().onKeyType(state)
    }
}
