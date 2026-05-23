package com.github.kavos113.karin.examples.state

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.*
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

            Box(
                style = Style.background(randomColor()),
                layout = Layout.size(width = 100f, height = 100f),
                event = Event.onClick {
                    println("Box clicked!")
                    println("text: ${text.value}")
                    text.value = "Clicked at ${System.currentTimeMillis()}"
                }
            )
            Text(
                text = text,
                fontSize = 24f
            )
        }
    }

    Karin.run()
}