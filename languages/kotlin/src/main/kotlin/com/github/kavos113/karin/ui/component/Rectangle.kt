package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.engine.handle.RectangleNodeHandle
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.common.Size

fun UiBuilder.Rectangle(size: Size, color: Color) {
    val handle = RectangleNodeHandle(size, color)
    parentContainer.addChild(handle)
}