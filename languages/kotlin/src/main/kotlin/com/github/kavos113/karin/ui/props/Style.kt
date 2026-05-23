package com.github.kavos113.karin.ui.props

import com.github.kavos113.karin.ui.common.Color

@ConsistentCopyVisibility
data class Style private constructor(
    val backgroundColor: Color? = null,
    val opacity: Float? = null,
) {
    fun background(color: Color) = copy(backgroundColor = color)
    fun alpha(value: Float) = copy(opacity = value)

    companion object {
        val Default = Style()

        fun background(color: Color) = Style().background(color)
        fun alpha(value: Float) = Style().alpha(value)
    }
}