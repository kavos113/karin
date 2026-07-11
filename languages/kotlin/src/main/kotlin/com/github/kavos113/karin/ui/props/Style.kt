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

    fun opacity(value: Float) = copy(opacity = value)
    fun opacity(state: State<Float>) = copy(opacityState = state)

    companion object {
        val Default = Style()

        fun background(color: Color) = Style().background(color)
        fun background(state: State<Color>) = Style().background(state)
        fun opacity(value: Float) = Style().opacity(value)
        fun opacity(state: State<Float>) = Style().opacity(state)
    }
}