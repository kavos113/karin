package com.github.kavos113.karin.ui.props

import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Color

@ConsistentCopyVisibility
data class Style private constructor(
    val backgroundColor: Color? = null,
    val backgroundColorState: State<Color>? = null,
    val opacity: Float? = null,
    val opacityState: State<Float>? = null
) {
    fun background(color: Color) = copy(backgroundColor = color)
    fun background(state: State<Color>) = copy(backgroundColorState = state)

    fun alpha(value: Float) = copy(opacity = value)
    fun alpha(state: State<Float>) = copy(opacityState = state)

    companion object {
        val Default = Style()

        fun background(color: Color) = Style().background(color)
        fun background(state: State<Color>) = Style().background(state)
        fun alpha(value: Float) = Style().alpha(value)
        fun alpha(state: State<Float>) = Style().alpha(state)
    }
}