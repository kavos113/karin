package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.engine.jni.JniViewNodeBridge
import com.github.kavos113.karin.engine.memory.NativeResourceManager
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.common.Point
import com.github.kavos113.karin.ui.common.Size
import com.github.kavos113.karin.ui.props.Event
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style
import java.lang.ref.Cleaner

internal open class ViewNodeHandle(ptr: Long) : ViewUpdateRequester {
    private var internalPtr = ptr
    private val cleanupTask = CleanupTask(ptr)
    private val cleanable: Cleaner.Cleanable = NativeResourceManager.cleaner.register(this, cleanupTask)

    enum class Side(val value: Int) {
        Left(0),
        Top(1),
        Right(2),
        Bottom(3),
        Horizontal(4),
        Vertical(5),
        All(6);
    }

    enum class EventType(val value: Int) {
        PointerMove(0),
        PointerDown(1),
        PointerUp(2),
        PointerEnter(3),
        PointerLeave(4),
        MouseWheel(5);
    }

    private var onPointerClick: (() -> Unit)? = null
    private var onPointerMove: ((Point) -> Unit)? = null
    private var onPointerDown: ((Point) -> Unit)? = null
    private var onPointerUp: ((Point) -> Unit)? = null
    private var onPointerEnter: (() -> Unit)? = null
    private var onPointerLeave: (() -> Unit)? = null
    private var onMouseWheel: ((Int) -> Unit)? = null
    private var onStartHover: (() -> Unit)? = null
    private var onEndHover: (() -> Unit)? = null
    private var onStartPress: (() -> Unit)? = null
    private var onEndPress: (() -> Unit)? = null

    private var isPressed = false
    private var isHovered = false

    val ptr: Long
        get() {
            check(internalPtr != 0L) {
                "this object is already destroyed, cannot access it anymore"
            }
            return internalPtr
        }

    fun transferOwnership() {
        check(internalPtr != 0L) {
            "this object is already destroyed, cannot transfer ownership again"
        }

        cleanupTask.isOwnedByCore = true
    }

    fun setOnClickListener(listener: () -> Unit) {
        onPointerClick = listener
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerUp.value)
    }

    fun setOnPointerMove(listener: (Point) -> Unit) {
        onPointerMove = listener
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerMove.value)
    }

    fun setOnPointerDown(listener: (Point) -> Unit) {
        onPointerDown = listener
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerDown.value)
    }

    fun setOnPointerUp(listener: (Point) -> Unit) {
        onPointerUp = listener
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerUp.value)
    }

    fun setOnPointerEnter(listener: () -> Unit) {
        onPointerEnter = listener
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerEnter.value)
    }

    fun setOnPointerLeave(listener: () -> Unit) {
        onPointerLeave = listener
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerLeave.value)
    }

    fun setOnMouseWheel(listener: (Int) -> Unit) {
        onMouseWheel = listener
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.MouseWheel.value)
    }

    fun setHoverHandler(start: () -> Unit, end: () -> Unit) {
        onStartHover = start
        onEndHover = end
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerEnter.value)
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerLeave.value)
    }

    fun setPressHandler(start: () -> Unit, end: () -> Unit) {
        onStartPress = start
        onEndPress = end
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerUp.value)
        JniViewNodeBridge.setPointerListener(ptr, this, EventType.PointerDown.value)
    }

    fun setSize(size: Size) {
        JniViewNodeBridge.setSize(ptr, size.width, size.height)
    }

    fun setWidth(width: Float) {
        JniViewNodeBridge.setWidth(ptr, width)
    }

    fun setHeight(height: Float) {
        JniViewNodeBridge.setHeight(ptr, height)
    }

    fun setBackgroundColor(color: Color) {
        JniViewNodeBridge.setBackgroundColor(ptr, color.r, color.g, color.b, color.a)
    }

    fun setOpacity(opacity: Float) {
        JniViewNodeBridge.setOpacity(ptr, opacity)
    }

    fun setMargin(flags: Char, left: Float, top: Float, right: Float, bottom: Float) {
        JniViewNodeBridge.setMargin(ptr, flags, left, top, right, bottom)
    }

    fun setPadding(flags: Char, left: Float, top: Float, right: Float, bottom: Float) {
        JniViewNodeBridge.setPadding(ptr, flags, left, top, right, bottom)
    }

    fun setMarginSide(side: Side, value: Float) {
        JniViewNodeBridge.setMarginSide(ptr, side.value, value)
    }

    fun setPaddingSide(side: Side, value: Float) {
        JniViewNodeBridge.setPaddingSide(ptr, side.value, value)
    }

    fun setBorder(side: Side, width: Float, color: Color, style: Int) {
        JniViewNodeBridge.setBorder(ptr, side.value, width, color.r, color.g, color.b, color.a, style)
    }

    fun setShadow(offsetX: Float, offsetY: Float, color: Color, blurRadius: Float, spreadRadius: Float) {
        JniViewNodeBridge.setShadow(ptr, offsetX, offsetY, color.r, color.g, color.b, color.a, blurRadius, spreadRadius)
    }

    override fun requestRelayout() {
        JniViewNodeBridge.requestRelayout(ptr)
    }

    override fun requestRedraw() {
        JniViewNodeBridge.requestRedraw(ptr)
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerMove")
    internal fun dispatchPointerMove(x: Float, y: Float) {
        onPointerMove?.invoke(Point(x, y))
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerDown")
    internal fun dispatchPointerDown(x: Float, y: Float) {
        isPressed = true
        onPointerDown?.invoke(Point(x, y))
        onStartPress?.invoke()
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerUp")
    internal fun dispatchPointerUp(x: Float, y: Float) {
        onPointerUp?.invoke(Point(x, y))

        if (isPressed && onPointerClick != null) {
            onPointerClick!!.invoke()
        }

        isPressed = false
        onEndPress?.invoke()
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerEnter")
    internal fun dispatchPointerEnter() {
        onPointerEnter?.invoke()
        onStartHover?.invoke()
        isHovered = true
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchPointerLeave")
    internal fun dispatchPointerLeave() {
        onPointerLeave?.invoke()
        onEndHover?.invoke()
        isHovered = false
    }

    @SuppressWarnings("unused")
    @JvmName("dispatchMouseWheel")
    internal fun dispatchMouseWheel(delta: Int) {
        onMouseWheel?.invoke(delta)
    }

    private class CleanupTask(private val ptr: Long) : Runnable {
        // if true, ptr also owned by c++ vector<ViewNode*> (ownership is kotlin)
        @Volatile
        var isOwnedByCore: Boolean = false

        override fun run() {
            if (ptr != 0L && !isOwnedByCore) {
                JniViewNodeBridge.destroy(ptr)
            }
        }
    }
}

internal fun ViewNodeHandle.applyStyle(style: Style) {
    style.backgroundColor?.let {
        setBackgroundColor(it)
    }

    style.backgroundColorState?.let { state ->
        state.onChange { color ->
            setBackgroundColor(color)
            requestRedraw()
        }
    }

    style.opacity?.let {
        setOpacity(it)
    }

    style.opacityState?.let { state ->
        state.onChange { opacity ->
            setOpacity(opacity)
            requestRedraw()
        }
    }

    style.border?.let {
        setBorder(ViewNodeHandle.Side.All, it.width, it.color, it.style.value)
    }

    style.borderState?.let { state ->
        state.onChange { border ->
            setBorder(ViewNodeHandle.Side.All, border.width, border.color, border.style.value)
            requestRedraw()
        }
    }

    style.borderTop?.let {
        setBorder(ViewNodeHandle.Side.Top, it.width, it.color, it.style.value)
    }

    style.borderTopState?.let { state ->
        state.onChange { border ->
            setBorder(ViewNodeHandle.Side.Top, border.width, border.color, border.style.value)
            requestRedraw()
        }
    }

    style.borderBottom?.let {
        setBorder(ViewNodeHandle.Side.Bottom, it.width, it.color, it.style.value)
    }

    style.borderBottomState?.let { state ->
        state.onChange { border ->
            setBorder(ViewNodeHandle.Side.Bottom, border.width, border.color, border.style.value)
            requestRedraw()
        }
    }

    style.borderLeft?.let {
        setBorder(ViewNodeHandle.Side.Left, it.width, it.color, it.style.value)
    }

    style.borderLeftState?.let { state ->
        state.onChange { border ->
            setBorder(ViewNodeHandle.Side.Left, border.width, border.color, border.style.value)
            requestRedraw()
        }
    }

    style.borderRight?.let {
        setBorder(ViewNodeHandle.Side.Right, it.width, it.color, it.style.value)
    }

    style.borderRightState?.let { state ->
        state.onChange { border ->
            setBorder(ViewNodeHandle.Side.Right, border.width, border.color, border.style.value)
            requestRedraw()
        }
    }

    style.shadow?.let {
        setShadow(it.offsetX, it.offsetY, it.color, it.blurRadius, it.spreadRadius)
    }

    style.shadowState?.let { state ->
        state.onChange { shadow ->
            setShadow(shadow.offsetX, shadow.offsetY, shadow.color, shadow.blurRadius, shadow.spreadRadius)
            requestRedraw()
        }
    }

    style.hoverStyle?.let {
        setHoverHandler(
            start = {
                applyStyle(it)
            },
            end = {
                applyStyle(style = style.copy(hoverStyle = null, pressedStyle = null))
            }
        )
    }
    style.pressedStyle?.let {
        setPressHandler(
            start = {
                applyStyle(it)
            },
            end = {
                applyStyle(style = style.copy(pressedStyle = null, hoverStyle = null))
            }
        )
    }
}

internal fun ViewNodeHandle.applyLayout(layout: Layout) {
    var flags = 0
    var top = 0f
    var bottom = 0f
    var left = 0f
    var right = 0f

    layout.paddingTop?.let {
        flags = flags or JniViewNodeBridge.SIDE_FLAG_TOP
        top = it
    }

    layout.paddingBottom?.let {
        flags = flags or JniViewNodeBridge.SIDE_FLAG_BOTTOM
        bottom = it
    }

    layout.paddingLeft?.let {
        flags = flags or JniViewNodeBridge.SIDE_FLAG_LEFT
        left = it
    }

    layout.paddingRight?.let {
        flags = flags or JniViewNodeBridge.SIDE_FLAG_RIGHT
        right = it
    }

    if (flags != 0) {
        setPadding(flags.toChar(), left, top, right, bottom)
    }

    flags = 0
    top = 0f
    bottom = 0f
    left = 0f
    right = 0f

    layout.marginTop?.let {
        flags = flags or JniViewNodeBridge.SIDE_FLAG_TOP
        top = it
    }

    layout.marginBottom?.let {
        flags = flags or JniViewNodeBridge.SIDE_FLAG_BOTTOM
        bottom = it
    }

    layout.marginLeft?.let {
        flags = flags or JniViewNodeBridge.SIDE_FLAG_LEFT
        left = it
    }

    layout.marginRight?.let {
        flags = flags or JniViewNodeBridge.SIDE_FLAG_RIGHT
        right = it
    }

    if (flags != 0) {
        setMargin(flags.toChar(), left, top, right, bottom)
    }

    layout.width?.let {
        setWidth(it)
    }

    layout.height?.let {
        setHeight(it)
    }

    layout.paddingTopState?.let { state ->
        state.onChange { value ->
            setPaddingSide(ViewNodeHandle.Side.Top, value)
            requestRelayout()
        }
    }

    layout.paddingBottomState?.let { state ->
        state.onChange { value ->
            setPaddingSide(ViewNodeHandle.Side.Bottom, value)
            requestRelayout()
        }
    }

    layout.paddingLeftState?.let { state ->
        state.onChange { value ->
            setPaddingSide(ViewNodeHandle.Side.Left, value)
            requestRelayout()
        }
    }

    layout.paddingRightState?.let { state ->
        state.onChange { value ->
            setPaddingSide(ViewNodeHandle.Side.Right, value)
            requestRelayout()
        }
    }

    layout.marginTopState?.let { state ->
        state.onChange { value ->
            setMarginSide(ViewNodeHandle.Side.Top, value)
            requestRelayout()
        }
    }

    layout.marginBottomState?.let { state ->
        state.onChange { value ->
            setMarginSide(ViewNodeHandle.Side.Bottom, value)
            requestRelayout()
        }
    }

    layout.marginLeftState?.let { state ->
        state.onChange { value ->
            setMarginSide(ViewNodeHandle.Side.Left, value)
            requestRelayout()
        }
    }

    layout.marginRightState?.let { state ->
        state.onChange { value ->
            setMarginSide(ViewNodeHandle.Side.Right, value)
            requestRelayout()
        }
    }

    layout.widthState?.let { state ->
        state.onChange { value ->
            setWidth(value)
            requestRelayout()
        }
    }

    layout.heightState?.let { state ->
        state.onChange { value ->
            setHeight(value)
            requestRelayout()
        }
    }
}

internal fun ViewNodeHandle.applyEvent(event: Event) {
    event.onClick?.let {
        setOnClickListener(it)
    }

    event.onClickState?.let { state ->
        state.onChange { handler ->
            setOnClickListener(handler ?: {})
        }
    }

    event.onPointerMove?.let {
        setOnPointerMove(it)
    }

    event.onPointerMoveState?.let { state ->
        state.onChange { handler ->
            setOnPointerMove(handler ?: {})
        }
    }

    event.onPointerDown?.let {
        setOnPointerDown(it)
    }

    event.onPointerDownState?.let { state ->
        state.onChange { handler ->
            setOnPointerDown(handler ?: {})
        }
    }

    event.onPointerUp?.let {
        setOnPointerUp(it)
    }

    event.onPointerUpState?.let { state ->
        state.onChange { handler ->
            setOnPointerUp(handler ?: {})
        }
    }

    event.onPointerEnter?.let {
        setOnPointerEnter(it)
    }

    event.onPointerEnterState?.let { state ->
        state.onChange { handler ->
            setOnPointerEnter(handler ?: {})
        }
    }

    event.onPointerLeave?.let {
        setOnPointerLeave(it)
    }

    event.onPointerLeaveState?.let { state ->
        state.onChange { handler ->
            setOnPointerLeave(handler ?: {})
        }
    }

    event.onMouseWheel?.let {
        setOnMouseWheel(it)
    }

    event.onMouseWheelState?.let { state ->
        state.onChange { handler ->
            setOnMouseWheel(handler ?: {})
        }
    }
}