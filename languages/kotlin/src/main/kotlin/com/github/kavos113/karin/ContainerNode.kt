package com.github.kavos113.karin

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.ui.LayoutDirection
import com.github.kavos113.karin.ui.WrapMode

open class ContainerNode internal constructor(
    handle: ContainerNodeHandle
): ViewNode(handle) {

    internal val containerHandle: ContainerNodeHandle
        get() = handle as ContainerNodeHandle

    constructor(): this(ContainerNodeHandle(JniContainerNodeBridge.create()))
    constructor(size: Size): this(ContainerNodeHandle(JniContainerNodeBridge.create(size.width, size.height)))

    fun addChild(child: ViewNode) {
        containerHandle.addChild(child.handle)
    }

    fun setLayoutDirection(direction: LayoutDirection) {
        containerHandle.setLayoutDirection(direction)
    }

    fun setWrapMode(wrapMode: WrapMode) {
        containerHandle.setWrapMode(wrapMode)
    }

    fun setGap(gap: Float) {
        containerHandle.setGap(gap)
    }
}