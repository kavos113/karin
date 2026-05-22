package com.github.kavos113.karin.examples.helloclickevent

import com.github.kavos113.karin.Karin
import com.github.kavos113.karin.Window
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.*
import com.github.kavos113.karin.ui.layout.LayoutWrap

fun randomColor(): Color {
    return Color(
        r = Math.random().toFloat(),
        g = Math.random().toFloat(),
        b = Math.random().toFloat(),
        a = 1f
    )
}

fun main() {
    val win = Window("Hello Click Event", 100, 100, 800, 600)

    win.setContent {
        Row(
            gap = 10f
        ) {
            for (i in 0 until 10) {
                Box(
                    style = Style.background(randomColor()),
                    layout = Layout.size(width = 100f, height = 100f)
                )
            }
            Row(
                style = Style.background(randomColor()),
                layout = Layout.size(width = 200f, height = 200f),
                event = Event.onClick {
                    println("Big rectangle clicked!")
                },
                gap = 5f,
                wrap = LayoutWrap.Wrap,
            ) {
                for (i in 0 until 5) {
                    Box(
                        style = Style.background(randomColor()),
                        layout = Layout.size(width = 50f, height = 50f)
                    )
                }
            }
        }
    }

    Karin.run()
}