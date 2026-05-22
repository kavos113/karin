package com.github.kavos113.karin.ui.component

data class Event(
    val onClick: (() -> Unit)? = null,
) {
    fun onClick(handler: () -> Unit) = copy(onClick = handler)

    companion object {
        val Default = Event()
    }
}
