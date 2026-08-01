package com.github.kavos113.karin.runtime

import com.github.kavos113.karin.engine.handle.ViewNodeHandle
import com.github.kavos113.karin.engine.handle.VirtualContainerNode
import com.github.kavos113.karin.ui.UiBuilder

fun <T, K> UiBuilder.ForEach(
    items: State<List<T>>,
    keySelector: (T) -> K,
    block: UiBuilder.(T) -> Unit
) {
    val components = mutableMapOf<K, List<ViewNodeHandle>>()

    for (item in items.value) {
        val key = keySelector(item)

        val virtualBuilder = object : UiBuilder() {
            override val parentContainer = VirtualContainerNode()
        }
        virtualBuilder.block(item)

        components[key] = virtualBuilder.parentContainer.children
    }

    for ((_, v) in components) {
        v.forEach { parentContainer.addChild(it) }
    }
}