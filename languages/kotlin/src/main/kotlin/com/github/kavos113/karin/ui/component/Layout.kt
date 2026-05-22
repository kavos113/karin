package com.github.kavos113.karin.ui.component

@ConsistentCopyVisibility
data class Layout private constructor(
    val paddingTop: Float? = null,
    val paddingBottom: Float? = null,
    val paddingLeft: Float? = null,
    val paddingRight: Float? = null,

    val marginTop: Float? = null,
    val marginBottom: Float? = null,
    val marginLeft: Float? = null,
    val marginRight: Float? = null,

    val width: Float? = null,
    val height: Float? = null,
) {
    fun padding(
        top: Float? = null,
        bottom: Float? = null,
        left: Float? = null,
        right: Float? = null,
    ) = copy(
        paddingTop = top ?: paddingTop,
        paddingBottom = bottom ?: paddingBottom,
        paddingLeft = left ?: paddingLeft,
        paddingRight = right ?: paddingRight,
    )

    fun padding(all: Float) = copy(
        paddingTop = all,
        paddingBottom = all,
        paddingLeft = all,
        paddingRight = all,
    )

    fun padding(
        horizontal: Float? = null,
        vertical: Float? = null,
    ) = copy(
        paddingTop = vertical ?: paddingTop,
        paddingBottom = vertical ?: paddingBottom,
        paddingLeft = horizontal ?: paddingLeft,
        paddingRight = horizontal ?: paddingRight,
    )

    fun margin(
        top: Float? = null,
        bottom: Float? = null,
        left: Float? = null,
        right: Float? = null,
    ) = copy(
        marginTop = top ?: marginTop,
        marginBottom = bottom ?: marginBottom,
        marginLeft = left ?: marginLeft,
        marginRight = right ?: marginRight,
    )

    fun margin(all: Float) = copy(
        marginTop = all,
        marginBottom = all,
        marginLeft = all,
        marginRight = all,
    )

    fun margin(
        horizontal: Float? = null,
        vertical: Float? = null,
    ) = copy(
        marginTop = vertical ?: marginTop,
        marginBottom = vertical ?: marginBottom,
        marginLeft = horizontal ?: marginLeft,
        marginRight = horizontal ?: marginRight,
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
        ) = Layout().padding(top, bottom, left, right)
        fun padding(all: Float) = Layout().padding(all)
        fun padding(
            horizontal: Float? = null,
            vertical: Float? = null,
        ) = Layout().padding(horizontal, vertical)

        fun margin(
            top: Float? = null,
            bottom: Float? = null,
            left: Float? = null,
            right: Float? = null,
        ) = Layout().margin(top, bottom, left, right)
        fun margin(all: Float) = Layout().margin(all)
        fun margin(
            horizontal: Float? = null,
            vertical: Float? = null,
        ) = Layout().margin(horizontal, vertical)

        fun size(width: Float? = null, height: Float? = null) = Layout().size(width, height)
        fun width(width: Float) = Layout().width(width)
        fun height(height: Float) = Layout().height(height)
    }
}