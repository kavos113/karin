package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.ui.common.Color

data class Style(
    val backgroundColor: Color? = null,
    val opacity: Float? = null,
) {
    fun background(color: Color) = copy(backgroundColor = color)
    fun alpha(value: Float) = copy(opacity = value)

    companion object {
        val Default = Style()
    }
}