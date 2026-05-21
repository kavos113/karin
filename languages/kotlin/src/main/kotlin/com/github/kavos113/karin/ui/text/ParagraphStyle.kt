package com.github.kavos113.karin.ui.text

data class ParagraphStyle(
    val textHorizontalAlignment: TextHorizontalAlignment = TextHorizontalAlignment.Left,
    val textVerticalAlignment: TextVerticalAlignment = TextVerticalAlignment.Top,
    val textTrim: TextTrim = TextTrim.None,
    val textWrap: TextWrap = TextWrap.None,
    val flowDirection: TextDirection = TextDirection.TopToBottom,
    val readingDirection: TextDirection = TextDirection.LeftToRight,
    val textLineSpacing: TextLineSpacing = TextLineSpacing.Proportional,
    val lineSpacingValue: Float = 1f,
    val baseLineOffset: Float = 1f
)