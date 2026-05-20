package com.github.kavos113.karin

import com.github.kavos113.karin.engine.jni.JniWindowBridge

class Window internal constructor(
    internal var nativePtr: Long
) : AutoCloseable {

    // 子ノードとして追加されたviewの所有権はC++側で持つ
    fun setRootView(view: ViewNode) {
        require(view.nativePtr != 0L) { "ViewNode has been destroyed or already add child of others" }

        JniWindowBridge.setRootView(nativePtr, view.nativePtr)
        view.nativePtr = 0L
    }

    override fun close() {
        if (nativePtr != 0L) {
            JniWindowBridge.destroy(nativePtr)
            nativePtr = 0L
        }
    }
}