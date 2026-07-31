package com.github.kavos113.karin.examples.foreach

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.runtime.ForEach
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.Box
import com.github.kavos113.karin.ui.component.Column
import com.github.kavos113.karin.ui.layout.LayoutWrap
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

            ForEach(colors, { it }) { color ->
                Box(
                    style = Style.background(color),
                    layout = Layout.size(width = 100.0f, height = 100.0f)
                )
            }

        }
    }

    Karin.run()
}