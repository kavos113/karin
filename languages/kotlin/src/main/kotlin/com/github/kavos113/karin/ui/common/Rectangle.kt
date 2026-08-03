package com.github.kavos113.karin.ui.common

data class Rectangle(
    val pos: Point,
    val size: Size
) {
    constructor(
        x: Float,
        y: Float,
        width: Float,
        height: Float
    ): this(
        pos = Point(x, y),
        size = Size(width, height)
    )
}
