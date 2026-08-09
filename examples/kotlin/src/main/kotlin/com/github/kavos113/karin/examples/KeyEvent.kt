package com.github.kavos113.karin.examples.keyevent

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.Box
import com.github.kavos113.karin.ui.component.Row
import com.github.kavos113.karin.ui.layout.LayoutWrap
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
    val win = Window("Hello Layout", 100, 100, 800, 600)

    win.setContent {
        Row(
            wrap = LayoutWrap.Wrap,
            gap = 10f,
            layout = Layout.margin(all = 10f)
        ) {
            val press = State(false)

            val color = randomColor()
            val activeColor = randomColor()

            Box(
                style = Style {
                    backgroundColor(press.map { if (it) { color } else { activeColor } })
                },
                layout = Layout.size(width = 100f, height = 100f),
                event = Event
                    .onClick {
                        press.value = !press.value
                        println("clicked")
                    }
                    .onKeyType { println("key: $it") }
            ) {

            }
        }
    }

    Karin.run()
}