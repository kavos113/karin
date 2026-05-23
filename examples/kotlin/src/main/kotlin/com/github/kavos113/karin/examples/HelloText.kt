package com.github.kavos113.karin.examples.hellotext

import com.github.kavos113.karin.ui.app.Karin
import com.github.kavos113.karin.ui.app.Window
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.*

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
            for (i in 0 until 10) {
                Box(
                    style = Style.background(randomColor()),
                    layout = Layout.size(width = 100f, height = 100f)
                )
            }
            Text(
                text = "Hello, Karin!",
                fontSize = 24f
            )
        }
    }

    Karin.run()
}