package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.ui.layout.LayoutDirection
import com.github.kavos113.karin.ui.layout.LayoutWrap

internal interface ContainerNode {
    fun addChild(child: ViewNodeHandle)
    fun removeChild(child: ViewNodeHandle)
    fun clearChildren()

    fun setLayoutDirection(direction: LayoutDirection)
    fun setLayoutWrap(layoutWrap: LayoutWrap)
    fun setGap(gap: Float)
    fun setEnableClip(enableClip: Boolean)
}