package com.github.kavos113.karin

class Application : AutoCloseable {
    internal var nativePtr: Long = JniApplication.create()

    fun createWindow(title: String, x: Int, y: Int, width: Int, height: Int): Window {
        val windowPtr = JniApplication.createWindow(nativePtr, title, x, y, width, height)
        return Window(windowPtr)
    }

    fun run() {
        JniApplication.run(nativePtr)
    }

    override fun close() {
        if (nativePtr != 0L) {
            JniApplication.destroy(nativePtr)
            nativePtr = 0L
        }
    }
}