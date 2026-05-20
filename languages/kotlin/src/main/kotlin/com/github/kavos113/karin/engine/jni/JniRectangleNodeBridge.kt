package com.github.kavos113.karin.engine.jni

internal object JniRectangleNodeBridge {
    fun create(width: Float, height: Float, r: Float, g: Float, b: Float, a: Float): Long = JniRectangleNode.create(width, height, r, g, b, a)
}