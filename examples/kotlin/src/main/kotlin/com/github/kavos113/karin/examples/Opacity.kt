package com.github.kavos113.karin.examples.opacity

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.component.Box
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.component.Row
import com.github.kavos113.karin.ui.props.Style
import com.github.kavos113.karin.ui.layout.LayoutWrap
import com.github.kavos113.karin.ui.props.Event

fun main() {
    val win = Window("Hello Layout", 100, 100, 800, 600)

    win.setContent {
        Row(
            wrap = LayoutWrap.Wrap,
            gap = 10f
        ) {
            for (i in 0 until 10) {
                Box(
                    style = Style
                        .background(Color(0x0000ffff))
                        .opacity(1.0f - i * 0.07f),
                    layout = Layout.size(width = 100f, height = 100f)
                )
            }

            val opacity: State<Float> = State(1.0f)

            Row(
                style = Style
                    .background(Color(0x00ffffff))
                    .opacity(opacity),
                layout = Layout.size(width = 200f, height = 200f),
                event = Event.onClick {
                    if (opacity.value < 1.0f) {
                        opacity.value = 1.0f
                    } else {
                        opacity.value = 0.2f;
                    }
                },
                wrap = LayoutWrap.Wrap,
                gap = 5f
            ) {
                for (i in 0 until 5) {
                    Box(
                        style = Style.background(Color(0x00ff00ff)),
                        layout = Layout.size(width = 50f, height = 50f)
                    )
                }
            }
        }
    }

    Karin.run()
}