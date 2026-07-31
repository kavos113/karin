package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.ui.common.Size

internal class ContainerNodeWithChildren private constructor(
    private val handle: ContainerNodeHandle
) : ContainerNode by handle {

    constructor(): this(ContainerNodeHandle())
    constructor(size: Size): this(ContainerNodeHandle(size = size))

    private val _children = mutableListOf<ViewNodeHandle>()
    internal val children: List<ViewNodeHandle>
        get() = _children

    override fun addChild(child: ViewNodeHandle) {
        _children.add(child)
        handle.addChild(child)
    }

    override fun removeChild(child: ViewNodeHandle) {
        _children.remove(child)
        handle.addChild(child)
    }

    override fun clearChildren() {
        _children.clear()
        handle.clearChildren()
    }
}