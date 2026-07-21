package com.github.kavos113.karin.examples.shadow

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.Box
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.component.Row
import com.github.kavos113.karin.ui.props.Style
import com.github.kavos113.karin.ui.layout.LayoutWrap
import com.github.kavos113.karin.ui.style.LineStyle

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
            layout = Layout.padding(all = 10f)
        ) {
            Box(
                style = Style
                    .background(randomColor())
                    .shadow(offsetX = 5.0f, offsetY = 5.0f, color = randomColor(), blurRadius = 10.0f),
                layout = Layout.size(width = 100f, height = 100f)
            )
        }
    }

    Karin.run()
}