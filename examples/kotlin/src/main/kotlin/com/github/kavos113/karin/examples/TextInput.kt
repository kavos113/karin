package com.github.kavos113.karin.examples.textinput

import com.github.kavos113.karin.app.Karin
import com.github.kavos113.karin.app.Window
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.component.Row
import com.github.kavos113.karin.ui.component.Text
import com.github.kavos113.karin.ui.component.TextInput
import com.github.kavos113.karin.ui.layout.LayoutWrap
import com.github.kavos113.karin.ui.props.Layout

fun main() {
    val win = Window("Hello Layout", 100, 100, 800, 600)

    win.setContent {
        Row(
            wrap = LayoutWrap.Wrap,
            gap = 10f,
            layout = Layout.margin(all = 10f)
        ) {
            val text = State("")

            TextInput(
                initialText = text.value,
                onTextChange = { str ->
                    text.value = str
                }
            )

            Text(text = text.map { "Your input: ${text.value}" }, fontSize = 24f)
        }
    }

    Karin.run()
}