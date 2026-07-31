package com.github.kavos113.karin.runtime

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.ui.UiBuilder

fun <T, K> UiBuilder.ForEach(
    items: State<List<T>>,
    keySelector: (T) -> K,
    block: UiBuilder.(T) -> Unit
) {
    for (item in items.value) {
        block(item)
    }
}