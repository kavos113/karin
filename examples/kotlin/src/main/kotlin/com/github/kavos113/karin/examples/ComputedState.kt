package com.github.kavos113.karin.examples.computedstate

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.runtime.computedState
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.Box
import com.github.kavos113.karin.ui.component.Row
import com.github.kavos113.karin.ui.component.Text
import com.github.kavos113.karin.ui.props.Event
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style

fun randomColor(): Color {
    return Color(
        r = Math.random().toFloat(),
        g = Math.random().toFloat(),
        b = Math.random().toFloat(),
        a = 1f
    )
}

fun main() {
    val win = Window("Hello Text", 100, 100, 800, 600)

    win.setContent {
        Row(
            gap = 10f,
        ) {
            val text: State<String> = State("Hello, Karin!")
            val count: State<Int> = State(0)
            val count2: State<Int> = State(1)

            Box(
                style = Style.background(randomColor()),
                layout = Layout.size(width = 100f, height = 100f),
                event = Event.onClick {
                    text.value = "Clicked at ${System.currentTimeMillis()}"
                    count.value++
                    count2.value += count.value
                }
            )
            Text(
                text = computedState {
                    if ((count.value + count2.value) % 5 == 0) {
                        return@computedState "oooo ${count.value + count2.value}"
                    }
                    return@computedState "count1: ${count.value}, count2: ${count2.value}"
                },
                fontSize = 24f
            )
        }
    }

    Karin.run()
}