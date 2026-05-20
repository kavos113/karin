package com.github.kavos113.karin.engine.jni

import com.github.kavos113.karin.engine.handle.ViewNodeHandle

internal object JniViewNodeBridge {
    fun setClickListener(nodePtr: Long, node: ViewNodeHandle) = JniViewNode.setClickListener(nodePtr, node)
    fun destroy(nodePtr: Long) = JniViewNode.destroy(nodePtr)
}