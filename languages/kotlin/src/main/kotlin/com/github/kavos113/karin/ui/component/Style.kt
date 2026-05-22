package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.ui.common.Color

data class Style(
    val backGroundColor: Color? = null,
    val opacity: Float? = null,
) {
    fun backGround(color: Color) = copy(backGroundColor = color)
    fun alpha(value: Float) = copy(opacity = value)

    companion object {
        val Default = Style()
    }
}