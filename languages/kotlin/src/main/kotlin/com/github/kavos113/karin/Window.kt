package com.github.kavos113.karin

import com.github.kavos113.karin.engine.handle.WindowHandle

class Window(
    title: String,
    x: Int,
    y: Int,
    width: Int,
    height: Int
) {
    private val handle: WindowHandle = WindowHandle(title, x, y, width, height)

    fun setRootView(view: ViewNode) {
        handle.setRootView(view.handle)
    }
}