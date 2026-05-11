package com.github.kavos113.karin

class RectangleNode internal constructor(ptr: Long) : ContainerNode(ptr) {

    constructor(size: Size, color: Color)
        : this(JniRectangleNode.create(size.width, size.height, color.r, color.g, color.b, color.a))
}