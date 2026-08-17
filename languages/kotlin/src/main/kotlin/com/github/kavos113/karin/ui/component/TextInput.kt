package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.engine.handle.TextNodeHandle
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.event.Key
import com.github.kavos113.karin.ui.internal.isControlChar
import com.github.kavos113.karin.ui.internal.unicodeLength
import com.github.kavos113.karin.ui.internal.unicodeSubstr
import com.github.kavos113.karin.ui.props.Event
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style
import com.github.kavos113.karin.ui.style.LineStyle
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextStyle

private val BACKGROUND_COLOR = Color(0xfdfdfdff)
private val BORDER_COLOR = Color(0x808080ff)
private val BORDER_HOVER_COLOR = Color(0x303030ff)
private val BORDER_FOCUS_COLOR = Color(0x3870d9ff)

private const val DEFAULT_WIDTH = 120f

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
    val caretIndex = State<Int>(initialText.unicodeLength())

    val textNodeHandle = TextNodeHandle(
        text = text.value,
        style = textStyle,
        paragraphStyle = paragraphStyle
    )
    textNodeHandle.setCaretIndex(caretIndex.value)

    val disposable = text.onChange {
        onTextChange(it)
    }

    val finalStyle: Style = if (style == Style.Default) {
        style
            .border(1f, BORDER_COLOR, LineStyle.Solid)
            .background(BACKGROUND_COLOR)
            .hover(Style.border(1f, BORDER_HOVER_COLOR, LineStyle.Solid))
            .focus(Style.border(1f, BORDER_FOCUS_COLOR, LineStyle.Solid))
    } else {
        style
    }

    val finalEvent = event
        .onKeyType {
            val codePoint = it.codePointAt(0)
            if (codePoint.isControlChar()) {
                return@onKeyType
            }

            text.value += it
            caretIndex.value += it.unicodeLength()
            println("current text: ${text.value}")
        }
        .onKeyDown {
            when (it.key) {
                Key.Backspace -> {
                    text.value = text.value.unicodeSubstr(0, text.value.unicodeLength() - 1)
                    if (caretIndex.value > 0) {
                        caretIndex.value--
                    }
                }
                Key.LeftArrow -> {
                    if (caretIndex.value > 0) {
                        caretIndex.value--
                    }
                }
                Key.RightArrow -> {
                    if (caretIndex.value < text.value.unicodeLength()) {
                        caretIndex.value++
                    }
                }
                // TODO: tab, delete, etc..
                else -> {}
            }
        }
        .onChangeFocus {
            textNodeHandle.setEnableCaret(it)
        }

    val height = layout.height ?: (textStyle.fontSize + 4)
    val width = layout.width ?: DEFAULT_WIDTH
    val finalLayout = layout
        .height(height)
        .width(width)

    Box(
        style = finalStyle,
        layout = finalLayout,
        event = finalEvent
    ) {
        parentContainer.addChild(textNodeHandle)
        childrenCount++

        val disposable = text.onChange { newText ->
            textNodeHandle.setText(newText)
            textNodeHandle.requestRedraw()
        }
        registerDisposable(disposable)

        val caretDisposable = caretIndex.onChange { newIndex ->
            textNodeHandle.setCaretIndex(newIndex)
            textNodeHandle.requestRedraw()
        }
        registerDisposable(caretDisposable)
    }

    registerDisposable(disposable)
}