package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.ui.common.Size

internal class ContainerNodeWithChildren private constructor(
    private val handle: ContainerNodeHandle
) : ContainerNode by handle {

    constructor(): this(ContainerNodeHandle())
    constructor(size: Size): this(ContainerNodeHandle(size = size))

    private val children = mutableListOf<ViewNodeHandle>()

    override fun addChild(child: ViewNodeHandle) {
        children.add(child)
        handle.addChild(child)
    }

    override fun removeChild(child: ViewNodeHandle) {
        children.remove(child)
        handle.addChild(child)
    }

    override fun clearChildren() {
        children.clear()
        handle.clearChildren()
    }
}