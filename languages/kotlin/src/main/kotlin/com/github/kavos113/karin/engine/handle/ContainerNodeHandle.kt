package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.ui.common.Size
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.ui.layout.LayoutDirection
import com.github.kavos113.karin.ui.layout.LayoutWrap

internal open class ContainerNodeHandle(ptr: Long) : ViewNodeHandle(ptr), ContainerNode {

    constructor(): this(JniContainerNodeBridge.create())
    constructor(size: Size): this(JniContainerNodeBridge.create(size.width, size.height))

    private val children = mutableListOf<ViewNodeHandle>()

    override fun addChild(child: ViewNodeHandle) {
        val childPtr = child.ptr
        JniContainerNodeBridge.addChild(this.ptr, childPtr)

        child.transferOwnership()

        children.add(child)
    }

    override fun insertChild(child: ViewNodeHandle, index: Int) {
        val childPtr = child.ptr
        JniContainerNodeBridge.insertChild(this.ptr, childPtr, index)

        child.transferOwnership()

        children.add(index, child)
    }

    override fun removeChild(child: ViewNodeHandle) {
        val childPtr = child.ptr
        JniContainerNodeBridge.removeChild(this.ptr, childPtr)

        children.remove(child)
    }

    override fun clearChildren() {
        JniContainerNodeBridge.clearChildren(this.ptr)

        children.clear()
    }

    override fun childIndexOf(child: ViewNodeHandle): Int {
        return children.indexOf(child)
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
}