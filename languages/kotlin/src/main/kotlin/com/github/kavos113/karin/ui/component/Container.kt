package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.handle.applyEvent
import com.github.kavos113.karin.engine.handle.applyLayout
import com.github.kavos113.karin.engine.handle.applyStyle
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.layout.LayoutDirection
import com.github.kavos113.karin.ui.layout.LayoutWrap
import com.github.kavos113.karin.ui.props.Event
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style

fun UiBuilder.Container(
    style: Style = Style.Default,
    layout: Layout = Layout.Default,
    event: Event = Event.Default,
    direction: LayoutDirection,
    wrap: LayoutWrap = LayoutWrap.Wrap,
    gap: Float = 0f,
    enableClip: Boolean = false,
    content: UiBuilder.() -> Unit
) {
    val handle = ContainerNodeHandle()
    handle.setLayoutDirection(direction)
    handle.setLayoutWrap(wrap)
    handle.setGap(gap)
    handle.setEnableClip(enableClip)

    handle.applyStyle(style)
    handle.applyLayout(layout)
    handle.applyEvent(event)

    val builder = object : UiBuilder() {
        override val parentContainer = handle
    }
    builder.content()

    parentContainer.addChild(handle)
}

fun UiBuilder.Column(
    style: Style = Style.Default,
    layout: Layout = Layout.Default,
    event: Event = Event.Default,
    wrap: LayoutWrap = LayoutWrap.Wrap,
    gap: Float = 0f,
    enableClip: Boolean = false,
    content: UiBuilder.() -> Unit
) = Container(
    style = style,
    layout = layout,
    event = event,
    direction = LayoutDirection.Column,
    wrap = wrap,
    gap = gap,
    enableClip = enableClip,
    content = content
)

fun UiBuilder.Row(
    style: Style = Style.Default,
    layout: Layout = Layout.Default,
    event: Event = Event.Default,
    wrap: LayoutWrap = LayoutWrap.Wrap,
    gap: Float = 0f,
    enableClip: Boolean = false,
    content: UiBuilder.() -> Unit
) = Container(
    style = style,
    layout = layout,
    event = event,
    direction = LayoutDirection.Row,
    wrap = wrap,
    gap = gap,
    enableClip = enableClip,
    content = content
)