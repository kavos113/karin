package com.github.kavos113.karin.ui.props

@ConsistentCopyVisibility
data class Event private constructor(
    val onClick: (() -> Unit)? = null,
) {
    fun onClick(handler: () -> Unit) = copy(onClick = handler)

    companion object {
        val Default = Event()

        fun onClick(handler: () -> Unit) = Event().onClick(handler)
    }
}
