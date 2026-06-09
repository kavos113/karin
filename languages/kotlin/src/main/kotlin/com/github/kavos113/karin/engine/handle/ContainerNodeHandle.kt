package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.ui.common.Size
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.ui.layout.LayoutDirection
import com.github.kavos113.karin.ui.layout.LayoutWrap

internal open class ContainerNodeHandle(ptr: Long) : ViewNodeHandle(ptr) {

    constructor(): this(JniContainerNodeBridge.create())
    constructor(size: Size): this(JniContainerNodeBridge.create(size.width, size.height))

    fun addChild(child: ViewNodeHandle) {
        val childPtr = child.ptr
        JniContainerNodeBridge.addChild(this.ptr, childPtr)

        child.transferOwnership()
    }

    fun setLayoutDirection(direction: LayoutDirection) {
        JniContainerNodeBridge.setLayoutDirection(this.ptr, direction.value)
    }

    fun setLayoutWrap(layoutWrap: LayoutWrap) {
        JniContainerNodeBridge.setWrapMode(this.ptr, layoutWrap.value)
    }

    fun setGap(gap: Float) {
        JniContainerNodeBridge.setGap(this.ptr, gap)
    }

    fun setEnableClip(enableClip: Boolean) {
        JniContainerNodeBridge.setEnableClip(this.ptr, enableClip)
    }

    fun removeChild(child: ViewNodeHandle) {
        val childPtr = child.ptr
        JniContainerNodeBridge.removeChild(this.ptr, childPtr)
    }

    fun clearChildren() {
        JniContainerNodeBridge.clearChildren(this.ptr)
    }
}