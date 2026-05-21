package com.github.kavos113.karin.examples.hellotext

import com.github.kavos113.karin.Color
import com.github.kavos113.karin.ContainerNode
import com.github.kavos113.karin.Karin
import com.github.kavos113.karin.RectangleNode
import com.github.kavos113.karin.Size
import com.github.kavos113.karin.TextNode
import com.github.kavos113.karin.TextStyle
import com.github.kavos113.karin.Window
import com.github.kavos113.karin.ui.layout.LayoutDirection
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
    val win = Window("Hello Text", 100, 100, 800, 600)

    val root = ContainerNode(Size(800f, 600f))

    for (i in 0 until 10) {
        val child = RectangleNode(Size(100f, 100f), randomColor())
        root.addChild(child)
    }

    val textNode = TextNode(
        text = "Hello, Karin!",
        style = TextStyle(
            fontSize = 24f
        )
    )
    root.addChild(textNode)

    root.setLayoutDirection(LayoutDirection.Row)
    root.setGap(10.0f)
    root.setWrapMode(LayoutWrap.Wrap)

    win.setRootView(root)

    Karin.run()
}