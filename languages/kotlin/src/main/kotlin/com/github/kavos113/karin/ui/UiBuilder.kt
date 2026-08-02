package com.github.kavos113.karin.ui

import com.github.kavos113.karin.engine.handle.ContainerNode
import com.github.kavos113.karin.engine.handle.ViewUpdateRequester

abstract class UiBuilder {
    internal abstract val parentContainer: ContainerNode
    internal abstract val viewUpdateRequester: ViewUpdateRequester
    internal var childrenCount = 0

    private val disposables = mutableListOf<() -> Unit>()

    internal fun registerDisposable(dispose: () -> Unit) {
        disposables.add(dispose)
    }

    internal fun dispose() {
        disposables.forEach { it() }
        disposables.clear()
    }
}