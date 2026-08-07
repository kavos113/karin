package com.github.kavos113.karin.examples.button

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.component.Button
import com.github.kavos113.karin.ui.component.Row
import com.github.kavos113.karin.ui.component.Text
import com.github.kavos113.karin.ui.layout.LayoutWrap
import com.github.kavos113.karin.ui.props.Layout

fun main() {
    val win = Window("Hello Layout", 100, 100, 800, 600)

    win.setContent {
        Row(
            wrap = LayoutWrap.Wrap,
            gap = 10f,
            layout = Layout.padding(10f)
        ) {
            val count = State(0)

            Button(text = "click!") {
                count.value++
            }

            Text(count.map { "clicked ${count.value} times" })
        }
    }

    Karin.run()
}