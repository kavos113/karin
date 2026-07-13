package com.github.kavos113.karin.examples.border

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
            gap = 10f
        ) {
            Box(
                style = Style
                    .background(randomColor())
                    .border(5.0f, Color(0, 0, 0), LineStyle.Solid),
                layout = Layout.size(width = 100f, height = 100f)
            )
            Box(
                style = Style
                    .background(randomColor())
                    .border(5.0f, Color(0, 255, 0), LineStyle.Dashed),
                layout = Layout.size(width = 100f, height = 100f)
            )
        }
    }

    Karin.run()
}