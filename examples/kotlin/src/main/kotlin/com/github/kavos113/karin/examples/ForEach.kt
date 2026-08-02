package com.github.kavos113.karin.examples.foreach

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.runtime.ForEach
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.Box
import com.github.kavos113.karin.ui.component.Column
import com.github.kavos113.karin.ui.component.Row
import com.github.kavos113.karin.ui.component.Text
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
        Column(
            wrap = LayoutWrap.Wrap,
            gap = 10f,
        ) {
            val colors = State((1 until 20).map { randomColor() })

            Box(
                style = Style.background(Color(200, 200, 200)),
                layout = Layout.size(width = 200.0f, 100.0f),
                event = Event.onClick {
                    val target = (0..18).random()

                    val list = colors.value.toMutableList()
                    list[target] = randomColor()

                    println("changed $target to ${colors.value[target]}")

                    colors.value = list.toList()
                }
            ) {
                Text("Click to change")
            }

            Row {
                ForEach(colors) { color ->
                    Box(
                        style = Style.background(color),
                        layout = Layout.size(width = 50.0f, height = 50.0f)
                    )
                }
            }

        }
    }

    Karin.run()
}