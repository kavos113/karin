package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.engine.handle.TextNodeHandle
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.event.Key
import com.github.kavos113.karin.ui.internal.isControlChar
import com.github.kavos113.karin.ui.internal.unicodeInsert
import com.github.kavos113.karin.ui.internal.unicodeLength
import com.github.kavos113.karin.ui.internal.unicodeRemove
import com.github.kavos113.karin.ui.internal.unicodeSubstr
import com.github.kavos113.karin.ui.props.Event
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style
import com.github.kavos113.karin.ui.style.LineStyle
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextStyle
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.FlowPreview
import kotlinx.coroutines.Job
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.debounce
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import kotlin.time.Duration.Companion.milliseconds

private val BACKGROUND_COLOR = Color(0xfdfdfdff)
private val BORDER_COLOR = Color(0x808080ff)
private val BORDER_HOVER_COLOR = Color(0x303030ff)
private val BORDER_FOCUS_COLOR = Color(0x3870d9ff)

private val CARET_BLINK_DURATION = 500.milliseconds
private val CARET_BLINK_DEBOUNCE = 1000.milliseconds

private const val DEFAULT_WIDTH = 120f

@OptIn(FlowPreview::class)
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
    val editingIndex = State<Int>(initialText.unicodeLength())

    val textNodeHandle = TextNodeHandle(
        text = text.value,
        style = textStyle,
        paragraphStyle = paragraphStyle
    )
    textNodeHandle.setCaretIndex(editingIndex.value)

    val disposable = text.onChange {
        onTextChange(it)
    }
    registerDisposable(disposable)

    val finalStyle: Style = if (style == Style.Default) {
        style
            .border(1f, BORDER_COLOR, LineStyle.Solid)
            .background(BACKGROUND_COLOR)
            .hover(Style.border(1f, BORDER_HOVER_COLOR, LineStyle.Solid))
            .focus(Style.border(1f, BORDER_FOCUS_COLOR, LineStyle.Solid))
    } else {
        style
    }

    var isEnableCaret = false
    var isFocused = false
    var blinkJob: Job? = null
    val scope = CoroutineScope(SupervisorJob() + Dispatchers.Default)
    val flow = MutableSharedFlow<Unit>(
        extraBufferCapacity = 1,
        onBufferOverflow = BufferOverflow.DROP_OLDEST
    )

    val startBlink = {
        blinkJob?.cancel()

        blinkJob = scope.launch {
            while (isActive) {
                isEnableCaret = !isEnableCaret
                textNodeHandle.setEnableCaret(isEnableCaret)
                textNodeHandle.requestRedraw()

                delay(CARET_BLINK_DURATION)
            }
        }
    }

    val stopBlink = {
        blinkJob?.cancel()
        blinkJob = null
    }

    scope.launch {
        flow
            .debounce(CARET_BLINK_DEBOUNCE)
            .collect {
                if (isFocused) {
                    startBlink()
                }
            }
    }

    val finalEvent = event
        .onKeyType {
            val codePoint = it.codePointAt(0)
            if (codePoint.isControlChar()) {
                return@onKeyType
            }

            text.value = text.value.unicodeInsert(editingIndex.value, it)
            editingIndex.value += it.unicodeLength()
            println("current text: ${text.value}")

            stopBlink()
            if (!isEnableCaret) {
                isEnableCaret = true
                textNodeHandle.setEnableCaret(true)
                textNodeHandle.requestRedraw()
            }

            flow.tryEmit(Unit)
        }
        .onKeyDown {
            when (it.key) {
                Key.Backspace -> {
                    if (editingIndex.value == 0) {
                        return@onKeyDown
                    }

                    text.value = text.value.unicodeRemove(editingIndex.value - 1, 1)
                    editingIndex.value--
                }
                Key.LeftArrow -> {
                    if (editingIndex.value > 0) {
                        editingIndex.value--
                    }
                }
                Key.RightArrow -> {
                    if (editingIndex.value < text.value.unicodeLength()) {
                        editingIndex.value++
                    }
                }
                // TODO: tab, delete, etc..
                else -> {}
            }

            stopBlink()
            if (!isEnableCaret) {
                isEnableCaret = true
                textNodeHandle.setEnableCaret(true)
                textNodeHandle.requestRedraw()
            }
            flow.tryEmit(Unit)
        }
        .onChangeFocus {
            isFocused = it

            if (it) {
                startBlink()
            } else {
                stopBlink()

                isEnableCaret = false
                textNodeHandle.setEnableCaret(false)
                textNodeHandle.requestRedraw()
            }
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

        val caretDisposable = editingIndex.onChange { newIndex ->
            textNodeHandle.setCaretIndex(newIndex)
            textNodeHandle.requestRedraw()
        }
        registerDisposable(caretDisposable)
    }
}