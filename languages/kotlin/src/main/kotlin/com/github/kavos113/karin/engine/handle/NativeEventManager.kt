package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.engine.jni.JniViewNodeBridge
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Point
import com.github.kavos113.karin.ui.event.Key
import com.github.kavos113.karin.ui.event.KeyEvent
import com.github.kavos113.karin.ui.event.KeyEventType
import com.github.kavos113.karin.ui.event.KeyModifierMask

internal class NativeEventManager(ptr: Long) : EventManager {
    private var internalPtr = ptr

    private var onPointerClick: (() -> Unit)? = null
    private var onPointerMove: ((Point) -> Unit)? = null
    private var onPointerDown: ((Point) -> Unit)? = null
    private var onPointerUp: ((Point) -> Unit)? = null
    private var onPointerEnter: (() -> Unit)? = null
    private var onPointerLeave: (() -> Unit)? = null
    private var onMouseWheel: ((Int) -> Unit)? = null
    private var onKeyDown: ((KeyEvent) -> Unit)? = null
    private var onKeyUp: ((KeyEvent) -> Unit)? = null
    private var onKeyType: ((String) -> Unit)? = null

    @JvmInline
    private value class NativeEventType private constructor(val value: Int) {
        companion object {
            val PointerMove = NativeEventType(0)
            val PointerDown = NativeEventType(1)
            val PointerUp = NativeEventType(2)
            val PointerEnter = NativeEventType(3)
            val PointerLeave = NativeEventType(4)
            val MouseWheel = NativeEventType(5)
            val Key = NativeEventType(6)
            val KeyType = NativeEventType(7)

            const val ENUM_COUNT = 8
        }
    }
    private var nativeHandlerEnabled = 0L

    override val isHovered: State<Boolean> = State(false)
    override val isPressed: State<Boolean> = State(false)
    override val isFocused: State<Boolean> = State(false)

    val ptr: Long
        get() {
            check(internalPtr != 0L) {
                "this object is already destroyed, cannot access it anymore"
            }
            return internalPtr
        }

    override fun enablePointerMoveHandler() {
        val mask = 1L shl NativeEventType.PointerMove.value;

        if (nativeHandlerEnabled and mask != 0L) {
            return
        }
        JniViewNodeBridge.setPointerMoveHandler(ptr, this)

        nativeHandlerEnabled = nativeHandlerEnabled or mask
    }

    override fun enablePointerDownHandler() {
        val mask = 1L shl NativeEventType.PointerDown.value;

        if (nativeHandlerEnabled and mask != 0L) {
            return
        }
        JniViewNodeBridge.setPointerDownHandler(ptr, this)

        nativeHandlerEnabled = nativeHandlerEnabled or mask
    }

    override fun enablePointerUpHandler() {
        val mask = 1L shl NativeEventType.PointerUp.value

        if (nativeHandlerEnabled and mask != 0L) {
            return
        }
        JniViewNodeBridge.setPointerUpHandler(ptr, this)

        nativeHandlerEnabled = nativeHandlerEnabled or mask
    }

    override fun enablePointerEnterHandler() {
        val mask = 1L shl NativeEventType.PointerEnter.value

        if (nativeHandlerEnabled and mask != 0L) {
            return
        }
        JniViewNodeBridge.setPointerEnterHandler(ptr, this)

        nativeHandlerEnabled = nativeHandlerEnabled or mask
    }

    override fun enablePointerLeaveHandler() {
        val mask = 1L shl NativeEventType.PointerLeave.value

        if (nativeHandlerEnabled and mask != 0L) {
            return
        }
        JniViewNodeBridge.setPointerLeaveHandler(ptr, this)

        nativeHandlerEnabled = nativeHandlerEnabled or mask
    }

    override fun enableMouseWheelHandler() {
        val mask = 1L shl NativeEventType.MouseWheel.value

        if (nativeHandlerEnabled and mask != 0L) {
            return
        }
        JniViewNodeBridge.setMouseWheelHandler(ptr, this)

        nativeHandlerEnabled = nativeHandlerEnabled or mask
    }

    override fun enableKeyHandler() {
        val mask = 1L shl NativeEventType.Key.value

        if (nativeHandlerEnabled and mask != 0L) {
            return
        }
        JniViewNodeBridge.setKeyHandler(ptr, this)

        nativeHandlerEnabled = nativeHandlerEnabled or mask
    }

    override fun enableKeyTypeHandler() {
        val mask = 1L shl NativeEventType.KeyType.value

        if (nativeHandlerEnabled and mask != 0L) {
            return
        }
        JniViewNodeBridge.setKeyTypeHandler(ptr, this)

        nativeHandlerEnabled = nativeHandlerEnabled or mask
    }

    private fun disablePointerMoveHandler() {
        val mask = 1L shl NativeEventType.PointerMove.value;

        if (nativeHandlerEnabled and mask == 0L) {
            return
        }
        JniViewNodeBridge.clearPointerMoveHandler(ptr)

        nativeHandlerEnabled = nativeHandlerEnabled and (mask.inv())
    }

    private fun disablePointerDownHandler() {
        val mask = 1L shl NativeEventType.PointerDown.value;

        if (nativeHandlerEnabled and mask == 0L) {
            return
        }
        JniViewNodeBridge.clearPointerDownHandler(ptr)

        nativeHandlerEnabled = nativeHandlerEnabled and (mask.inv())
    }

    private fun disablePointerUpHandler() {
        val mask = 1L shl NativeEventType.PointerUp.value

        if (nativeHandlerEnabled and mask == 0L) {
            return
        }
        JniViewNodeBridge.clearPointerUpHandler(ptr)

        nativeHandlerEnabled = nativeHandlerEnabled and (mask.inv())
    }

    private fun disablePointerEnterHandler() {
        val mask = 1L shl NativeEventType.PointerEnter.value

        if (nativeHandlerEnabled and mask == 0L) {
            return
        }
        JniViewNodeBridge.clearPointerEnterHandler(ptr)

        nativeHandlerEnabled = nativeHandlerEnabled and (mask.inv())
    }

    private fun disablePointerLeaveHandler() {
        val mask = 1L shl NativeEventType.PointerLeave.value

        if (nativeHandlerEnabled and mask == 0L) {
            return
        }
        JniViewNodeBridge.clearPointerLeaveHandler(ptr)

        nativeHandlerEnabled = nativeHandlerEnabled and (mask.inv())
    }

    private fun disableMouseWheelHandler() {
        val mask = 1L shl NativeEventType.MouseWheel.value

        if (nativeHandlerEnabled and mask == 0L) {
            return
        }
        JniViewNodeBridge.clearMouseWheelHandler(ptr)

        nativeHandlerEnabled = nativeHandlerEnabled and (mask.inv())
    }

    private fun disableKeyHandler() {
        val mask = 1L shl NativeEventType.Key.value

        if (nativeHandlerEnabled and mask == 0L) {
            return
        }
        JniViewNodeBridge.clearKeyHandler(ptr)

        nativeHandlerEnabled = nativeHandlerEnabled and (mask.inv())
    }

    private fun disableKeyTypeHandler() {
        val mask = 1L shl NativeEventType.KeyType.value

        if (nativeHandlerEnabled and mask == 0L) {
            return
        }
        JniViewNodeBridge.clearKeyTypeHandler(ptr)

        nativeHandlerEnabled = nativeHandlerEnabled and (mask.inv())
    }

    override fun setOnClick(handler: () -> Unit) {
        onPointerClick = handler
        enablePointerDownHandler()
        enablePointerUpHandler()
    }

    override fun setOnPointerMove(handler: (Point) -> Unit) {
        onPointerMove = handler
        enablePointerMoveHandler()
    }

    override fun setOnPointerDown(handler: (Point) -> Unit) {
        onPointerDown = handler
        enablePointerDownHandler()
    }

    override fun setOnPointerUp(handler: (Point) -> Unit) {
        onPointerUp = handler
        enablePointerUpHandler()
    }

    override fun setOnPointerEnter(handler: () -> Unit) {
        onPointerEnter = handler
        enablePointerEnterHandler()
    }

    override fun setOnPointerLeave(handler: () -> Unit) {
        onPointerLeave = handler
        enablePointerLeaveHandler()
    }

    override fun setOnMouseWheel(handler: (Int) -> Unit) {
        onMouseWheel = handler
        enableMouseWheelHandler()
    }

    override fun applyEventState() {
        if (isHovered.active()) {
            enablePointerEnterHandler()
            enablePointerLeaveHandler()
        }
        if (isPressed.active()) {
            enablePointerUpHandler()
            enablePointerDownHandler()
        }
        if (isFocused.active()) {
            setIsFocusable(true)
        }
    }

    override fun setOnKeyDown(handler: (KeyEvent) -> Unit) {
        onKeyDown = handler
        enableKeyHandler()
    }

    override fun setOnKeyUp(handler: (KeyEvent) -> Unit) {
        onKeyUp = handler
        enableKeyHandler()
    }

    override fun setOnKeyType(handler: (String) -> Unit) {
        onKeyType = handler
        enableKeyTypeHandler()
    }

    override fun setIsFocusable(isFocusable: Boolean) {
        JniViewNodeBridge.setIsFocusable(ptr, isFocusable, this)

        if (isFocusable) {
            enablePointerDownHandler()
            enablePointerUpHandler()
        }
    }

    @SuppressWarnings("unused")
    @JvmName("onChangeFocusState")
    internal fun onChangeFocusState(focused: Boolean) {
        val oldFocused = isFocused
        isFocused.value = focused
    }

    override fun clearOnClick() {
        onPointerClick = null

        if (onKeyDown == null && onKeyUp == null && onKeyType == null) {
            if (onPointerDown == null) {
                disablePointerDownHandler()
            }
            if (onPointerUp == null) {
                disablePointerUpHandler()
            }
        }
    }

    override fun clearOnPointerMove() {
        onPointerMove = null

        disablePointerMoveHandler()
    }

    override fun clearOnPointerDown() {
        onPointerDown = null

        if (onKeyDown == null && onKeyUp == null && onKeyType == null && onPointerClick == null && !isPressed.active()) {
            disablePointerDownHandler()
        }
    }

    override fun clearOnPointerUp() {
        onPointerUp = null

        if (onKeyDown == null && onKeyUp == null && onKeyType == null && onPointerClick == null && !isPressed.active()) {
            disablePointerUpHandler()
        }
    }

    override fun clearOnPointerEnter() {
        onPointerEnter = null

        if (!isHovered.active()) {
            disablePointerEnterHandler()
        }
    }

    override fun clearOnPointerLeave() {
        onPointerLeave = null

        if (!isHovered.active()) {
            disablePointerLeaveHandler()
        }
    }

    override fun clearOnMouseWheel() {
        onMouseWheel = null

        disableMouseWheelHandler()
    }

    override fun clearOnKeyDown() {
        onKeyDown = null

        if (onKeyUp == null) {
            disableKeyHandler()
        }
    }

    override fun clearOnKeyUp() {
        onKeyUp = null

        if (onKeyDown == null) {
            disableKeyHandler()
        }
    }

    override fun clearOnKeyType() {
        onKeyType = null

        disableKeyTypeHandler()
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerMove")
    internal fun dispatchPointerMove(x: Float, y: Float) {
        onPointerMove?.invoke(Point(x, y))
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerDown")
    internal fun dispatchPointerDown(x: Float, y: Float) {
        isPressed.value = true
        onPointerDown?.invoke(Point(x, y))
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerUp")
    internal fun dispatchPointerUp(x: Float, y: Float) {
        onPointerUp?.invoke(Point(x, y))

        if (isPressed.value) {
            onPointerClick?.invoke()
        }

        isPressed.value = false
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerEnter")
    internal fun dispatchPointerEnter() {
        isHovered.value = true
        onPointerEnter?.invoke()
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerLeave")
    internal fun dispatchPointerLeave() {
        isHovered.value = false
        onPointerLeave?.invoke()
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchMouseWheel")
    internal fun dispatchMouseWheel(delta: Int) {
        onMouseWheel?.invoke(delta)
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchKeyEvent")
    internal fun dispatchKeyEvent(type: Int, keyCode: Int, scanCode: Int, modifier: Int) {
        val event = KeyEvent(
            key = Key.fromInt(keyCode),
            physicalKey = Key.fromInt(scanCode),
            modifiers = KeyModifierMask(modifier)
        )

        when(KeyEventType.fromInt(type)) {
            KeyEventType.KeyPress -> {
                onKeyDown?.invoke(event)
            }
            KeyEventType.KeyRelease -> {
                onKeyUp?.invoke(event)
            }
        }
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchKeyType")
    internal fun dispatchKeyType(character: String) {
        onKeyType?.invoke(character)
    }
}