package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.engine.jni.JniViewNodeBridge
import com.github.kavos113.karin.engine.memory.NativeResourceManager
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.common.Size
import com.github.kavos113.karin.ui.component.Event
import com.github.kavos113.karin.ui.component.Layout
import com.github.kavos113.karin.ui.component.Style
import java.lang.ref.Cleaner

internal open class ViewNodeHandle(ptr: Long) {
    private var internalPtr = ptr
    private val cleanupTask = CleanupTask(ptr)
    private val cleanable: Cleaner.Cleanable = NativeResourceManager.cleaner.register(this, cleanupTask)

    private var onClick: (() -> Unit)? = null

    val ptr: Long
        get() {
            check(internalPtr != 0L) {
                "ownership of this handle already transferred to C++ side (or destroyed), cannot access it anymore"
            }
            return internalPtr
        }

    fun transferOwnership() {
        check(internalPtr != 0L) {
            "ownership of this handle already transferred to C++ side (or destroyed), cannot transfer ownership again"
        }

        cleanupTask.isOwnershipTransferred = true
        internalPtr = 0L
    }

    fun setOnClickListener(listener: () -> Unit) {
        onClick = listener
        JniViewNodeBridge.setClickListener(ptr, this)
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

    fun setMargin(flags: Char, left: Float, top: Float, right: Float, bottom: Float) {
        JniViewNodeBridge.setMargin(ptr, flags, left, top, right, bottom)
    }

    fun setPadding(flags: Char, left: Float, top: Float, right: Float, bottom: Float) {
        JniViewNodeBridge.setPadding(ptr, flags, left, top, right, bottom)
    }

    @JvmName("dispatchClickEvent")
    internal fun dispatchClickEvent() {
        onClick?.invoke()
    }

    private class CleanupTask(private val ptr: Long) : Runnable {
        @Volatile
        var isOwnershipTransferred: Boolean = false

        override fun run() {
            if (ptr != 0L && !isOwnershipTransferred) {
                JniViewNodeBridge.destroy(ptr)
            }
        }
    }
}

internal fun ViewNodeHandle.applyStyle(style: Style) {
    style.backgroundColor?.let {
        setBackgroundColor(it)
    }

    // TODO
}

internal fun ViewNodeHandle.applyLayout(layout: Layout) {
    var flags = 0
    var top = 0f
    var bottom = 0f
    var left = 0f
    var right = 0f

    layout.paddingTop?.let {
        flags = flags or JniViewNodeBridge.MARGIN_FLAG_TOP
        top = it
    }

    layout.paddingBottom?.let {
        flags = flags or JniViewNodeBridge.MARGIN_FLAG_BOTTOM
        bottom = it
    }

    layout.paddingLeft?.let {
        flags = flags or JniViewNodeBridge.MARGIN_FLAG_LEFT
        left = it
    }

    layout.paddingRight?.let {
        flags = flags or JniViewNodeBridge.MARGIN_FLAG_RIGHT
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
        flags = flags or JniViewNodeBridge.MARGIN_FLAG_TOP
        top = it
    }

    layout.marginBottom?.let {
        flags = flags or JniViewNodeBridge.MARGIN_FLAG_BOTTOM
        bottom = it
    }

    layout.marginLeft?.let {
        flags = flags or JniViewNodeBridge.MARGIN_FLAG_LEFT
        left = it
    }

    layout.marginRight?.let {
        flags = flags or JniViewNodeBridge.MARGIN_FLAG_RIGHT
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
}

internal fun ViewNodeHandle.applyEvent(event: Event) {
    event.onClick?.let {
        setOnClickListener(it)
    }
}