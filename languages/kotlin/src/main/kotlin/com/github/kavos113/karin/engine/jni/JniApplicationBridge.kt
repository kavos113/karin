package com.github.kavos113.karin.engine.jni

object JniApplicationBridge {
    fun create(): Long = JniApplication.create()
    fun createWindow(appPtr: Long, title: String, x: Int, y: Int, width: Int, height: Int): Long = JniApplication.createWindow(appPtr, title, x, y, width, height)
    fun run(appPtr: Long) = JniApplication.run(appPtr)
    fun destroy(appPtr: Long) = JniApplication.destroy(appPtr)
}