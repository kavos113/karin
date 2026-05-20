package com.github.kavos113.karin

import com.github.kavos113.karin.engine.jni.JniApplicationBridge

class Application : AutoCloseable {
    internal var nativePtr: Long = JniApplicationBridge.create()

    fun createWindow(title: String, x: Int, y: Int, width: Int, height: Int): Window {
        val windowPtr = JniApplicationBridge.createWindow(nativePtr, title, x, y, width, height)
        return Window(windowPtr)
    }

    fun run() {
        JniApplicationBridge.run(nativePtr)
    }

    override fun close() {
        if (nativePtr != 0L) {
            JniApplicationBridge.destroy(nativePtr)
            nativePtr = 0L
        }
    }
}