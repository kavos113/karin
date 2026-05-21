package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.ui.layout.LayoutDirection
import com.github.kavos113.karin.ui.layout.LayoutWrap

fun UiBuilder.Container(
    direction: LayoutDirection,
    wrap: LayoutWrap = LayoutWrap.Wrap,
    gap: Float = 0f,
    content: UiBuilder.() -> Unit
) {
    val handle = ContainerNodeHandle()
    handle.setLayoutDirection(direction)
    handle.setLayoutWrap(wrap)
    handle.setGap(gap)

    parentContainer.addChild(handle)

    val builder = object : UiBuilder() {
        override val parentContainer = handle
    }
    builder.content()
}

fun UiBuilder.Column(
    wrap: LayoutWrap = LayoutWrap.Wrap,
    gap: Float = 0f,
    content: UiBuilder.() -> Unit
) = Container(LayoutDirection.Column, wrap, gap, content)

fun UiBuilder.Row(
    wrap: LayoutWrap = LayoutWrap.Wrap,
    gap: Float = 0f,
    content: UiBuilder.() -> Unit
) = Container(LayoutDirection.Row, wrap, gap, content)