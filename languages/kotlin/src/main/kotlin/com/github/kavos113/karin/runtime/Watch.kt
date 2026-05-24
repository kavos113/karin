package com.github.kavos113.karin.runtime

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.ui.UiBuilder

fun <T> UiBuilder.Watch(
    state: State<T>,
    block: UiBuilder.(T) -> Unit
) {
    val containerNodeHandle = ContainerNodeHandle()

    var childBuilder: UiBuilder? = null

    val disposable = state.onChange { newValue ->
        childBuilder?.dispose()
        containerNodeHandle.clearChildren()

        val builder = object : UiBuilder() {
            override val parentContainer = containerNodeHandle
        }
        childBuilder = builder

        builder.block(newValue)
        containerNodeHandle.requestRelayout()
    }

    parentContainer.addChild(containerNodeHandle)
    registerDisposable(disposable)
}