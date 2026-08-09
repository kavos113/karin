package com.github.kavos113.karin.app

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.handle.WindowHandle
import com.github.kavos113.karin.ui.UiBuilder

class Window(
    title: String,
    x: Int,
    y: Int,
    width: Int,
    height: Int
) {
    private val handle: WindowHandle = WindowHandle(title, x, y, width, height)
    private val rootNode: ContainerNodeHandle = ContainerNodeHandle()

    fun setContent(
        content: UiBuilder.() -> Unit
    ) {
        val builder = object : UiBuilder() {
            override val parentContainer = rootNode
            override val viewUpdateRequester = rootNode
        }
        builder.content()

        handle.setRootView(rootNode)
    }
}