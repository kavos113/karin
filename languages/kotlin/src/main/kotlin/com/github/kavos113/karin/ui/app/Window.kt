package com.github.kavos113.karin.ui.app

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.handle.WindowHandle
import com.github.kavos113.karin.ui.component.UiBuilder

class Window(
    title: String,
    x: Int,
    y: Int,
    width: Int,
    height: Int
) {
    private val handle: WindowHandle = WindowHandle(title, x, y, width, height)

    fun setContent(
        content: UiBuilder.() -> Unit
    ) {
        val root = ContainerNodeHandle()

        val builder = object : UiBuilder() {
            override val parentContainer = root
        }
        builder.content()

        handle.setRootView(root)
    }
}