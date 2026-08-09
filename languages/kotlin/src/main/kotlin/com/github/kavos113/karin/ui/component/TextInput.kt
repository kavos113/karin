package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.props.Event
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style
import com.github.kavos113.karin.ui.style.LineStyle
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextStyle

private val BACKGROUND_COLOR = Color(0xfdfdfdff)
private val BORDER_COLOR = Color(0x808080ff)
private val BORDER_HOVER_COLOR = Color(0x303030ff)
// TODO: focus color

fun UiBuilder.TextInput(
    initialText: String,
    style: Style = Style.Default,
    layout: Layout = Layout.Default,
    event: Event = Event.Default,
    textStyle: TextStyle = TextStyle(),
    paragraphStyle: ParagraphStyle = ParagraphStyle(),
    onTextChange: (String) -> Unit,
) {
    val text = State<String>(initialText)

    text.onChange {
        onTextChange(it)
    }

    var finalStyle: Style = style
    if (style == Style.Default) {
        finalStyle = style
            .border(1f, BORDER_COLOR, LineStyle.Solid)
            .background(BACKGROUND_COLOR)
            .hover(Style.border(1f, BORDER_HOVER_COLOR, LineStyle.Solid))
    }

    val finalEvent = event.onKeyDown {
        text.value += it
    }

    Box(
        style = finalStyle,
        layout = layout,
        event = finalEvent
    ) {
        Text(
            text = text,
            style = textStyle,
            paragraphStyle = paragraphStyle
        )
    }
}