package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.ui.common.Size
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.ui.layout.LayoutDirection
import com.github.kavos113.karin.ui.layout.LayoutWrap

internal open class ContainerNodeHandle(ptr: Long) : ViewNodeHandle(ptr), ContainerNode {

    constructor(): this(JniContainerNodeBridge.create())
    constructor(size: Size): this(JniContainerNodeBridge.create(size.width, size.height))

    override fun addChild(child: ViewNodeHandle) {
        val childPtr = child.ptr
        JniContainerNodeBridge.addChild(this.ptr, childPtr)

        child.transferOwnership()
    }

    override fun setLayoutDirection(direction: LayoutDirection) {
        JniContainerNodeBridge.setLayoutDirection(this.ptr, direction.value)
    }

    override fun setLayoutWrap(layoutWrap: LayoutWrap) {
        JniContainerNodeBridge.setWrapMode(this.ptr, layoutWrap.value)
    }

    override fun setGap(gap: Float) {
        JniContainerNodeBridge.setGap(this.ptr, gap)
    }

    override fun setEnableClip(enableClip: Boolean) {
        JniContainerNodeBridge.setEnableClip(this.ptr, enableClip)
    }

    override fun removeChild(child: ViewNodeHandle) {
        val childPtr = child.ptr
        JniContainerNodeBridge.removeChild(this.ptr, childPtr)
    }

    override fun clearChildren() {
        JniContainerNodeBridge.clearChildren(this.ptr)
    }
}