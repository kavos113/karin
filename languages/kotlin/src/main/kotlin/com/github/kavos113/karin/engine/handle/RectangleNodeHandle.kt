package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.Color
import com.github.kavos113.karin.Size
import com.github.kavos113.karin.engine.jni.JniRectangleNodeBridge

internal class RectangleNodeHandle(ptr: Long) : ContainerNodeHandle(ptr) {

    constructor(size: Size, color: Color)
        : this(JniRectangleNodeBridge.create(size.width, size.height, color.r, color.g, color.b, color.a))
}