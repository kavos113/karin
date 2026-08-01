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

    items.onChange { newItems ->
        val newKeys = newItems.map(keySelector).toSet()

        // remove
        val keysToRemove = components.keys.filter { it !in newKeys }

        for (key in keysToRemove) {
            val nodesToRemove = components[key] ?: continue
            for (node in nodesToRemove) {
                parentContainer.removeChild(node)
            }
        }
    }
}