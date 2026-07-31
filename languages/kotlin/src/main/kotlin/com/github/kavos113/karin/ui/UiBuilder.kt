package com.github.kavos113.karin.ui

import com.github.kavos113.karin.engine.handle.ContainerNode
import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.handle.TextNodeHandle
import com.github.kavos113.karin.ui.common.Size
import com.github.kavos113.karin.ui.text.ParagraphStyle
import com.github.kavos113.karin.ui.text.TextStyle

abstract class UiBuilder {
    internal abstract val parentContainer: ContainerNode

    private val disposables = mutableListOf<() -> Unit>()

    internal fun registerDisposable(dispose: () -> Unit) {
        disposables.add(dispose)
    }

    internal fun dispose() {
        disposables.forEach { it() }
        disposables.clear()
    }

    internal open fun newContainerNodeHandle(): ContainerNode
        = ContainerNodeHandle()

    internal open fun newContainerNodeHandle(size: Size): ContainerNode
        = ContainerNodeHandle(size)

    internal open fun newTextNodeHandle(
        text: String = "",
        textStyle: TextStyle = TextStyle(),
        paragraphStyle: ParagraphStyle = ParagraphStyle()
    ) = TextNodeHandle(text, textStyle, paragraphStyle)
}