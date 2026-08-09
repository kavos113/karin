package com.github.kavos113.karin.app

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.handle.WindowHandle
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.event.Key
import com.github.kavos113.karin.ui.event.KeyEvent

class Window(
    title: String,
    x: Int,
    y: Int,
    width: Int,
    height: Int
) {
    private val handle: WindowHandle = WindowHandle(title, x, y, width, height)
    private val rootNode: ContainerNodeHandle = ContainerNodeHandle()

    private val keyHandlers: MutableList<(KeyEvent) -> Unit> = mutableListOf()
    private val shortcutHandlers: MutableMap<Key, () -> Unit> = mutableMapOf()

    private val keyHandler: (KeyEvent) -> Unit = { e ->
        for (h in keyHandlers) {
            h(e)
        }

        if (shortcutHandlers.containsKey(e.key)) {
            shortcutHandlers[e.key]!!()
        }
    }

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

    fun addKeyHandler(handler: (KeyEvent) -> Unit) {
        keyHandlers.add(handler)

        rootNode.setKeyDownHandler(keyHandler)
    }

    fun addShortcut(key: Key, handler: () -> Unit) {
        shortcutHandlers[key] = handler

        rootNode.setKeyDownHandler(keyHandler)
    }
}