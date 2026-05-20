package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.Size
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.ui.LayoutDirection
import com.github.kavos113.karin.ui.WrapMode

internal open class ContainerNodeHandle(ptr: Long) : ViewNodeHandle(ptr) {

    constructor(): this(JniContainerNodeBridge.create())
    constructor(size: Size): this(JniContainerNodeBridge.create(size.width, size.height))

    fun addChild(child: ViewNodeHandle) {
        val childPtr = child.transferOwnership()

        JniContainerNodeBridge.addChild(this.ptr, childPtr)
    }

    fun setLayoutDirection(direction: LayoutDirection) {
        JniContainerNodeBridge.setLayoutDirection(this.ptr, direction.value)
    }

    fun setWrapMode(wrapMode: WrapMode) {
        JniContainerNodeBridge.setWrapMode(this.ptr, wrapMode.value)
    }

    fun setGap(gap: Float) {
        JniContainerNodeBridge.setGap(this.ptr, gap)
    }
}