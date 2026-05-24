package com.github.kavos113.karin.examples.watch

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.runtime.Watch
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
            val counter = State(0)

            Box(
                style = Style.background(randomColor()),
                layout = Layout.size(width = 100f, height = 100f),
                event = Event.onClick {
                    println("Box clicked!")
                    println("counter: ${counter.value}")
                    counter.value += 1
                }
            )
            Watch(counter) { value ->
                when (value) {
                    in 0..5 -> Text(
                        text = "Counter: $value",
                        fontSize = 24f,
                        color = Color(0f, 1f, 0f, 1f)
                    )

                    in 6..10 -> Text(
                        text = "Counter: $value",
                        fontSize = 24f,
                        color = Color(1f, 1f, 0f, 1f)
                    )

                    else -> Text(
                        text = "Counter: $value",
                        fontSize = 24f,
                        color = Color(1f, 0f, 0f, 1f)
                    )
                }
            }
        }
    }

    Karin.run()
}