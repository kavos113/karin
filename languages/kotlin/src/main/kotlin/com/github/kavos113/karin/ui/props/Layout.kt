package com.github.kavos113.karin.ui.props

import com.github.kavos113.karin.runtime.State

@ConsistentCopyVisibility
data class Layout private constructor(
    val paddingTop: Float? = null,
    val paddingTopState: State<Float>? = null,
    val paddingBottom: Float? = null,
    val paddingBottomState: State<Float>? = null,
    val paddingLeft: Float? = null,
    val paddingLeftState: State<Float>? = null,
    val paddingRight: Float? = null,
    val paddingRightState: State<Float>? = null,

    val marginTop: Float? = null,
    val marginTopState: State<Float>? = null,
    val marginBottom: Float? = null,
    val marginBottomState: State<Float>? = null,
    val marginLeft: Float? = null,
    val marginLeftState: State<Float>? = null,
    val marginRight: Float? = null,
    val marginRightState: State<Float>? = null,

    val width: Float? = null,
    val widthState: State<Float>? = null,
    val height: Float? = null,
    val heightState: State<Float>? = null,
) {
    fun padding(
        top: Float? = null,
        bottom: Float? = null,
        left: Float? = null,
        right: Float? = null,
        all: Float? = null,
        horizontal: Float? = null,
        vertical: Float? = null,
    ) = copy(
        paddingTop = top ?: vertical ?: all ?: this.paddingTop,
        paddingBottom = bottom ?: vertical ?: all ?: this.paddingBottom,
        paddingLeft = left ?: horizontal ?: all ?: this.paddingLeft,
        paddingRight = right ?: horizontal ?: all ?: this.paddingRight,
    )

    fun paddingState(
        top: State<Float>? = null,
        bottom: State<Float>? = null,
        left: State<Float>? = null,
        right: State<Float>? = null,
        all: State<Float>? = null,
        horizontal: State<Float>? = null,
        vertical: State<Float>? = null,
    ) = copy(
        paddingTopState = top ?: vertical ?: all ?: this.paddingTopState,
        paddingBottomState = bottom ?: vertical ?: all ?: this.paddingBottomState,
        paddingLeftState = left ?: horizontal ?: all ?: this.paddingLeftState,
        paddingRightState = right ?: horizontal ?: all ?: this.paddingRightState,
    )

    fun margin(
        top: Float? = null,
        bottom: Float? = null,
        left: Float? = null,
        right: Float? = null,
        all: Float? = null,
        horizontal: Float? = null,
        vertical: Float? = null,
    ) = copy(
        marginTop = top ?: vertical ?: all ?: this.marginTop,
        marginBottom = bottom ?: vertical ?: all ?: this.marginBottom,
        marginLeft = left ?: horizontal ?: all ?: this.marginLeft,
        marginRight = right ?: horizontal ?: all ?: this.marginRight,
    )

    fun marginState(
        top: State<Float>? = null,
        bottom: State<Float>? = null,
        left: State<Float>? = null,
        right: State<Float>? = null,
        all: State<Float>? = null,
        horizontal: State<Float>? = null,
        vertical: State<Float>? = null,
    ) = copy(
        marginTopState = top ?: vertical ?: all ?: this.marginTopState,
        marginBottomState = bottom ?: vertical ?: all ?: this.marginBottomState,
        marginLeftState = left ?: horizontal ?: all ?: this.marginLeftState,
        marginRightState = right ?: horizontal ?: all ?: this.marginRightState,
    )

    fun size(width: Float? = null, height: Float? = null) = copy(
        width = width ?: this.width,
        height = height ?: this.height,
    )

    fun width(width: Float) = copy(width = width)
    fun height(height: Float) = copy(height = height)

    companion object {
        val Default = Layout()

        fun padding(
            top: Float? = null,
            bottom: Float? = null,
            left: Float? = null,
            right: Float? = null,
            all: Float? = null,
            horizontal: Float? = null,
            vertical: Float? = null,
        ) = Layout().padding(top, bottom, left, right, all, horizontal, vertical)

        fun paddingState(
            top: State<Float>? = null,
            bottom: State<Float>? = null,
            left: State<Float>? = null,
            right: State<Float>? = null,
            all: State<Float>? = null,
            horizontal: State<Float>? = null,
            vertical: State<Float>? = null,
        ) = Layout().paddingState(top, bottom, left, right, all, horizontal, vertical)

        fun margin(
            top: Float? = null,
            bottom: Float? = null,
            left: Float? = null,
            right: Float? = null,
            all: Float? = null,
            horizontal: Float? = null,
            vertical: Float? = null,
        ) = Layout().margin(top, bottom, left, right, all, horizontal, vertical)

        fun marginState(
            top: State<Float>? = null,
            bottom: State<Float>? = null,
            left: State<Float>? = null,
            right: State<Float>? = null,
            all: State<Float>? = null,
            horizontal: State<Float>? = null,
            vertical: State<Float>? = null,
        ) = Layout().marginState(top, bottom, left, right, all, horizontal, vertical)

        fun size(width: Float? = null, height: Float? = null) = Layout().size(width, height)
        fun width(width: Float) = Layout().width(width)
        fun height(height: Float) = Layout().height(height)
    }
}