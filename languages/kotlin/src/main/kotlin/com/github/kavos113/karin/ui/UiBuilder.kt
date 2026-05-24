package com.github.kavos113.karin.ui

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle

abstract class UiBuilder {
    internal abstract val parentContainer: ContainerNodeHandle

    private val disposables = mutableListOf<() -> Unit>()

    internal fun registerDisposable(dispose: () -> Unit) {
        disposables.add(dispose)
    }

    internal fun dispose() {
        disposables.forEach { it() }
        disposables.clear()
    }
}