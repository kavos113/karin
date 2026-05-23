package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.handle.applyEvent
import com.github.kavos113.karin.engine.handle.applyLayout
import com.github.kavos113.karin.engine.handle.applyStyle
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.props.Event
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style

fun UiBuilder.Box(
    style: Style = Style.Default,
    layout: Layout = Layout.Default,
    event: Event = Event.Default,
    content: (UiBuilder.() -> Unit)? = null
) {
    val handle = ContainerNodeHandle()

    handle.applyStyle(style)
    handle.applyLayout(layout)
    handle.applyEvent(event)

    if (content != null) {
        val builder = object : UiBuilder() {
            override val parentContainer = handle
        }
        builder.content()
    }

    parentContainer.addChild(handle)
}