package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.engine.jni.JniApplicationBridge

internal object ApplicationHandle {
    val ptr: Long = JniApplicationBridge.create()

    fun run() {
        JniApplicationBridge.run(ptr)
    }
}