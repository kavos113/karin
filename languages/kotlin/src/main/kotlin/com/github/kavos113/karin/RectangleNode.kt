package com.github.kavos113.karin

import com.github.kavos113.karin.engine.handle.RectangleNodeHandle
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.common.Size

class RectangleNode(size: Size, color: Color) : ContainerNode(
    RectangleNodeHandle(size, color)
) {

}