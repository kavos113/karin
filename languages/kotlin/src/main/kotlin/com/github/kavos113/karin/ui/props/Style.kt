package com.github.kavos113.karin.ui.props

import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.style.Border
import com.github.kavos113.karin.ui.style.LineStyle

@ConsistentCopyVisibility
data class Style private constructor(
    val backgroundColor: Color? = null,
    val backgroundColorState: State<Color>? = null,
    val opacity: Float? = null,
    val opacityState: State<Float>? = null,

    val border: Border? = null,
    val borderState: State<Border>? = null,
    val borderTop: Border? = null,
    val borderTopState: State<Border>? = null,
    val borderBottom: Border? = null,
    val borderBottomState: State<Border>? = null,
    val borderLeft: Border? = null,
    val borderLeftState: State<Border>? = null,
    val borderRight: Border? = null,
    val borderRightState: State<Border>? = null,
) {
    fun background(color: Color) = copy(backgroundColor = color)
    fun background(state: State<Color>) = copy(backgroundColorState = state)

    fun opacity(value: Float) = copy(opacity = value)
    fun opacity(state: State<Float>) = copy(opacityState = state)

    fun border(width: Float, color: Color, style: LineStyle) = copy(border = Border(width, color, style))
    fun borderState(state: State<Border>) = copy(borderState = state)
    fun borderTop(width: Float, color: Color, style: LineStyle) = copy(borderTop = Border(width, color, style))
    fun borderTopState(state: State<Border>) = copy(borderTopState = state)
    fun borderBottom(width: Float, color: Color, style: LineStyle) = copy(borderBottom = Border(width, color, style))
    fun borderBottomState(state: State<Border>) = copy(borderBottomState = state)
    fun borderLeft(width: Float, color: Color, style: LineStyle) = copy(borderLeft = Border(width, color, style))
    fun borderLeftState(state: State<Border>) = copy(borderLeftState = state)
    fun borderRight(width: Float, color: Color, style: LineStyle) = copy(borderRight = Border(width, color, style))
    fun borderRightState(state: State<Border>) = copy(borderRightState = state)

    companion object {
        val Default = Style()

        fun background(color: Color) = Style().background(color)
        fun background(state: State<Color>) = Style().background(state)
        fun opacity(value: Float) = Style().opacity(value)
        fun opacity(state: State<Float>) = Style().opacity(state)
        fun border(width: Float, color: Color, style: LineStyle) = Style().border(width, color, style)
        fun borderState(state: State<Border>) = Style().borderState(state)
        fun borderTop(width: Float, color: Color, style: LineStyle) = Style().borderTop(width, color, style)
        fun borderTopState(state: State<Border>) = Style().borderTopState(state)
        fun borderBottom(width: Float, color: Color, style: LineStyle) = Style().borderBottom(width, color, style)
        fun borderBottomState(state: State<Border>) = Style().borderBottomState(state)
        fun borderLeft(width: Float, color: Color, style: LineStyle) = Style().borderLeft(width, color, style)
        fun borderLeftState(state: State<Border>) = Style().borderLeftState(state)
        fun borderRight(width: Float, color: Color, style: LineStyle) = Style().borderRight(width, color, style)
        fun borderRightState(state: State<Border>) = Style().borderRightState(state)
    }
}