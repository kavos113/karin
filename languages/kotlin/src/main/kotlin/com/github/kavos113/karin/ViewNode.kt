package com.github.kavos113.karin

import com.github.kavos113.karin.engine.jni.JniViewNodeBridge

abstract class ViewNode(internal var nativePtr: Long) {

    private var onClick: (() -> Unit)? = null

    fun setOnClickListener(listener: () -> Unit) {
        onClick = listener
//        JniViewNodeBridge.setClickListener(nativePtr, this)
    }

    @JvmName("dispatchClickEvent")
    internal fun dispatchClickEvent() {
        onClick?.invoke()
    }
}