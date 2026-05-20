package com.github.kavos113.karin

import com.github.kavos113.karin.engine.handle.ViewNodeHandle

abstract class ViewNode internal constructor(
    internal val handle: ViewNodeHandle
){
    fun setOnClickListener(onClick: () -> Unit) {
        handle.setOnClickListener(onClick)
    }
}