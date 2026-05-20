package com.github.kavos113.karin

import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge

open class ContainerNode internal constructor(ptr: Long) : ViewNode(ptr) {

    enum class LayoutDirection(val value: Int) {
        Row(0),
        Column(1)
    }

    enum class WrapMode(val value: Int) {
        NoWrap(0),
        Wrap(1),
        WrapReverse(2)
    }

    constructor(): this(JniContainerNodeBridge.create())
    constructor(size: Size) : this(JniContainerNodeBridge.create(size.width, size.height))

    fun addChild(child: ViewNode) {
        require(child.nativePtr != 0L) { "ViewNode has been destroyed or already add child of others" }

        JniContainerNodeBridge.addChild(nativePtr, child.nativePtr)
        child.nativePtr = 0L
    }

    fun setLayoutDirection(direction: LayoutDirection) {
        JniContainerNodeBridge.setLayoutDirection(nativePtr, direction.value)
    }

    fun setWrapMode(wrapMode: WrapMode) {
        JniContainerNodeBridge.setWrapMode(nativePtr, wrapMode.value)
    }

    fun setGap(gap: Float) {
        JniContainerNodeBridge.setGap(nativePtr, gap)
    }
}