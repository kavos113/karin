package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.ui.common.Size
import com.github.kavos113.karin.ui.layout.LayoutDirection
import com.github.kavos113.karin.ui.layout.LayoutWrap

internal class VirtualContainerNode : ContainerNode {

    constructor()
    constructor(size: Size)

    private val _children = mutableListOf<ViewNodeHandle>()
    internal val children: List<ViewNodeHandle>
        get() = _children

    override fun addChild(child: ViewNodeHandle) {
        _children.add(child)
    }

    override fun removeChild(child: ViewNodeHandle) {
        _children.remove(child)
    }

    override fun clearChildren() {
        _children.clear()
    }

    override fun setLayoutDirection(direction: LayoutDirection) {

    }

    override fun setLayoutWrap(layoutWrap: LayoutWrap) {

    }

    override fun setGap(gap: Float) {

    }

    override fun setEnableClip(enableClip: Boolean) {

    }
}