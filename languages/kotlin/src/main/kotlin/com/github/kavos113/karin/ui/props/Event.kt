package com.github.kavos113.karin.ui.props

import com.github.kavos113.karin.runtime.State

@ConsistentCopyVisibility
data class Event private constructor(
    val onClick: (() -> Unit)? = null,
    val onClickState: State<(() -> Unit)?>? = null
) {
    fun onClick(handler: () -> Unit) = copy(onClick = handler)
    fun onClick(state: State<(() -> Unit)?>) = copy(onClickState = state)

    companion object {
        val Default = Event()

        fun onClick(handler: () -> Unit) = Event().onClick(handler)
        fun onClick(state: State<(() -> Unit)?>) = Event().onClick(state)
    }
}
