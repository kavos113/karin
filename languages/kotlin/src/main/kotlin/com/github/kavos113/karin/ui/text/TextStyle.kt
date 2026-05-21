package com.github.kavos113.karin.ui.text

import com.github.kavos113.karin.ui.common.Color

data class TextStyle(
    val fontSize: Float = 16f,
    val fontFamily: String = "Arial",
    val fontWeight: FontWeight = FontWeight.Normal,
    val fontStyle: FontStyle = FontStyle.Normal,
    val fontStretch: FontStretch = FontStretch.Normal,
    val color: Color = Color.Black,
    val locale: String = "en-US"
)
