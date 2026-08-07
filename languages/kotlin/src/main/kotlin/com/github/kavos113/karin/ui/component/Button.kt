package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.props.Event
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextStyle

private val BUTTON_COLOR = Color(0xf0f0f0ff)
private val BUTTON_COLOR_HOVER = Color(0xe0e0e0ff)
private val BUTTON_COLOR_PRESS = Color(0xd0d0d0ff)

fun UiBuilder.Button(
    onClick: () -> Unit = { },
    style: Style = Style.Default,
    layout: Layout = Layout.Default,
    event: Event = Event.Default,
    content: (UiBuilder.() -> Unit)
) {
    val finalEvent = event.onClick(onClick)

    var finalStyle: Style = style
    if (style.backgroundColor == null) {
        finalStyle = style
            .background(BUTTON_COLOR)
            .hover(Style.background(BUTTON_COLOR_HOVER))
            .pressed(Style.background(BUTTON_COLOR_PRESS))
    }

    Box(
        style = finalStyle,
        layout = layout,
        event = finalEvent,
    ) {
        content()
    }
}

fun UiBuilder.Button(
    style: Style = Style.Default,
    layout: Layout = Layout.Default,
    event: Event = Event.Default,
    text: String = "",
    textStyle: TextStyle = TextStyle(),
    paragraphStyle: ParagraphStyle = ParagraphStyle(),
    onClick: () -> Unit
) {
    val finalEvent = event.onClick(onClick)

    var finalStyle: Style = style
    if (style.backgroundColor == null) {
        finalStyle = style
            .background(BUTTON_COLOR)
            .hover(Style.background(BUTTON_COLOR_HOVER))
            .pressed(Style.background(BUTTON_COLOR_PRESS))
    }

    Box(
        style = finalStyle,
        layout = layout,
        event = finalEvent,
    ) {
        Text(
            text = text,
            style = textStyle,
            paragraphStyle = paragraphStyle
        )
    }
}

fun UiBuilder.Button(
    style: Style = Style.Default,
    layout: Layout = Layout.Default,
    event: Event = Event.Default,
    text: State<String> = State(""),
    textStyle: TextStyle = TextStyle(),
    paragraphStyle: ParagraphStyle = ParagraphStyle(),
    onClick: () -> Unit
) {
    val finalEvent = event.onClick(onClick)

    var finalStyle: Style = style
    if (style.backgroundColor == null) {
        finalStyle = style
            .background(BUTTON_COLOR)
            .hover(Style.background(BUTTON_COLOR_HOVER))
            .pressed(Style.background(BUTTON_COLOR_PRESS))
    }

    Box(
        style = finalStyle,
        layout = layout,
        event = finalEvent,
    ) {
        Text(
            text = text,
            style = textStyle,
            paragraphStyle = paragraphStyle
        )
    }
}