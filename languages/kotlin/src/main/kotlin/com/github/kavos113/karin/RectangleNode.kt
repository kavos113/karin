package com.github.kavos113.karin

import com.github.kavos113.karin.engine.jni.JniRectangleNodeBridge

class RectangleNode internal constructor(ptr: Long) : ContainerNode(ptr) {

    constructor(size: Size, color: Color)
        : this(JniRectangleNodeBridge.create(size.width, size.height, color.r, color.g, color.b, color.a))
}