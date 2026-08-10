package com.github.kavos113.karin.ui.props

import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.style.Border
import com.github.kavos113.karin.ui.style.Shadow

class StyleBuilder {
    internal var backgroundColor: Color? = null
    internal var backgroundColorState: State<Color>? = null

    fun backgroundColor(color: Color) {
        backgroundColor = color
    }

    fun backgroundColor(state: State<Color>) {
        backgroundColorState = state
    }

    internal var opacity: Float? = null
    internal var opacityState: State<Float>? = null

    fun opacity(value: Float) {
        opacity = value
    }

    fun opacity(state: State<Float>) {
        opacityState = state
    }

    internal var border: Border? = null
    internal var borderState: State<Border>? = null

    fun border(value: Border) {
        border = value
    }

    fun border(state: State<Border>) {
        borderState = state
    }

    internal var borderTop: Border? = null
    internal var borderTopState: State<Border>? = null

    fun borderTop(value: Border) {
        borderTop = value
    }

    fun borderTop(state: State<Border>) {
        borderTopState = state
    }

    internal var borderBottom: Border? = null
    internal var borderBottomState: State<Border>? = null

    fun borderBottom(value: Border) {
        borderBottom = value
    }

    fun borderBottom(state: State<Border>) {
        borderBottomState = state
    }

    internal var borderLeft: Border? = null
    internal var borderLeftState: State<Border>? = null

    fun borderLeft(value: Border) {
        borderLeft = value
    }

    fun borderLeft(state: State<Border>) {
        borderLeftState = state
    }

    internal var borderRight: Border? = null
    internal var borderRightState: State<Border>? = null

    fun borderRight(value: Border) {
        borderRight = value
    }

    fun borderRight(state: State<Border>) {
        borderRightState = state
    }

    internal var shadow: Shadow? = null
    internal var shadowState: State<Shadow>? = null

    fun shadow(value: Shadow) {
        shadow = value
    }

    fun shadow(state: State<Shadow>) {
        shadowState = state
    }

    internal var hoverStyle: StyleBuilder? = null
    internal var pressedStyle: StyleBuilder? = null
    internal var focusStyle: StyleBuilder? = null

    fun hover(block: StyleBuilder.() -> Unit) {
        hoverStyle = StyleBuilder().apply(block)
    }

    fun pressed(block: StyleBuilder.() -> Unit) {
        pressedStyle = StyleBuilder().apply(block)
    }

    fun focus(block: StyleBuilder.() -> Unit) {
        focusStyle = StyleBuilder().apply(block)
    }

    internal fun build(): Style {
        return Style(
            backgroundColor = backgroundColor,
            backgroundColorState = backgroundColorState,
            opacity = opacity,
            opacityState = opacityState,
            border = border,
            borderState = borderState,
            borderTop = borderTop,
            borderTopState = borderTopState,
            borderBottom = borderBottom,
            borderBottomState = borderBottomState,
            borderLeft = borderLeft,
            borderLeftState = borderLeftState,
            borderRight = borderRight,
            borderRightState = borderRightState,
            shadow = shadow,
            shadowState = shadowState,
            hoverStyle = hoverStyle?.build(),
            pressedStyle = pressedStyle?.build(),
            focusStyle = focusStyle?.build(),
        )
    }
}

fun Style(block: StyleBuilder.() -> Unit): Style {
    val builder = StyleBuilder().apply(block)
    return builder.build()
}