package com.github.kavos113.karin.engine.jni

internal object JniWindowBridge {
    fun setRootView(windowPtr: Long, viewPtr: Long) = JniWindow.setRootView(windowPtr, viewPtr)
    fun destroy(windowPtr: Long) = JniWindow.destroy(windowPtr)
}