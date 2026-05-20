package com.github.kavos113.karin.engine.jni

object JniContainerNodeBridge {
    fun create(): Long = JniContainerNode.create()
    fun create(width: Float, height: Float): Long = JniContainerNode.create(width, height)
    fun addChild(containerPtr: Long, childPtr: Long) = JniContainerNode.addChild(containerPtr, childPtr)
    fun setLayoutDirection(containerPtr: Long, direction: Int) = JniContainerNode.setLayoutDirection(containerPtr, direction)
    fun setGap(containerPtr: Long, gap: Float) = JniContainerNode.setGap(containerPtr, gap)
    fun setWrapMode(containerPtr: Long, wrapMode: Int) = JniContainerNode.setWrapMode(containerPtr, wrapMode)
}