package com.github.kavos113.karin.examples.helloclickevent

import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ContainerNode
import com.github.kavos113.karin.Karin
import com.github.kavos113.karin.RectangleNode
import com.github.kavos113.karin.ui.common.Size
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
    val win = Window("Hello Click Event", 100, 100, 800, 600)

    val root = ContainerNode(Size(800f, 600f))

    for (i in 0 until 10) {
        val child = RectangleNode(Size(100f, 100f), randomColor())
        root.addChild(child)
    }

    val bigRect = RectangleNode(Size(200f, 200f), randomColor())
    for (i in 0 until 5) {
        val child = RectangleNode(Size(50f, 50f), randomColor())
        bigRect.addChild(child)
    }
    bigRect.setLayoutDirection(LayoutDirection.Column)
    bigRect.setGap(5.0f)
    bigRect.setWrapMode(LayoutWrap.Wrap)
    bigRect.setOnClickListener {
        println("Big rectangle clicked!")
    }
    root.addChild(bigRect)

    root.setLayoutDirection(LayoutDirection.Row)
    root.setGap(10.0f)
    root.setWrapMode(LayoutWrap.Wrap)

    win.setRootView(root)

    Karin.run()
}