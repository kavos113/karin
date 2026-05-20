package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.engine.jni.JniApplicationBridge
import com.github.kavos113.karin.engine.jni.JniWindowBridge
import com.github.kavos113.karin.engine.memory.NativeResourceManager

internal class WindowHandle(
    title: String,
    x: Int,
    y: Int,
    width: Int,
    height: Int
) {
    private val ptr: Long = JniApplicationBridge.createWindow(
        ApplicationHandle.ptr,
        title,
        x,
        y,
        width,
        height
    )

    init {
        NativeResourceManager.cleaner.register(this, CleanupTask(ptr))
    }

    fun setRootView(view: ViewNodeHandle) {
        val viewPtr = view.transferOwnership()
        JniWindowBridge.setRootView(ptr, viewPtr)
    }

    private class CleanupTask(private val ptr: Long) : Runnable {
        override fun run() {
            JniWindowBridge.destroy(ptr)
        }
    }
}