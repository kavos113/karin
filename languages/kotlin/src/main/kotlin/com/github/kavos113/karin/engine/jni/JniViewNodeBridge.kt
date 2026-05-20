package com.github.kavos113.karin.engine.jni

import com.github.kavos113.karin.ViewNode

object JniViewNodeBridge {
    fun setClickListener(nodePtr: Long, node: ViewNode) = JniViewNode.setClickListener(nodePtr, node)
}