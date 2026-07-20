package com.github.kavos113.karin.ui.style

import com.github.kavos113.karin.ui.common.Color

data class Shadow(
    val offsetX: Float,
    val offsetY: Float,
    val color: Color,
    val blurRadius: Float = 0.0f,
    val spreadRadius: Float = 0.0f,
)
