package com.github.kavos113.karin.ui.component

data class Layout(
    val padding: Float? = null,
) {
    fun padding(value: Float) = copy(padding = value)

    companion object {
        val Default = Layout()
    }
}