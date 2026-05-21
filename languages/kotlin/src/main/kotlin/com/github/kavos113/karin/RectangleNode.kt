package com.github.kavos113.karin

import com.github.kavos113.karin.engine.handle.RectangleNodeHandle

class RectangleNode(size: Size, color: Color) : ContainerNode(
    RectangleNodeHandle(size, color)
) {

}