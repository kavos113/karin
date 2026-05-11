package com.github.kavos113.karin

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

    constructor(): this(JniContainerNode.create())
    constructor(size: Size) : this(JniContainerNode.create(size.width, size.height))

    fun addChild(child: ViewNode) {
        require(child.nativePtr != 0L) { "ViewNode has been destroyed or already add child of others" }

        JniContainerNode.addChild(nativePtr, child.nativePtr)
        child.nativePtr = 0L
    }

    fun setLayoutDirection(direction: LayoutDirection) {
        JniContainerNode.setLayoutDirection(nativePtr, direction.value)
    }

    fun setWrapMode(wrapMode: WrapMode) {
        JniContainerNode.setWrapMode(nativePtr, wrapMode.value)
    }

    fun setGap(gap: Float) {
        JniContainerNode.setGap(nativePtr, gap)
    }
}