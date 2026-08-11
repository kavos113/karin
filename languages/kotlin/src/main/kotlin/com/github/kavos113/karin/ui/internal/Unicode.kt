package com.github.kavos113.karin.ui.internal

internal fun Int.isControlChar(): Boolean {
    return this <= 0x1F || (this in 0x7F..0x9F)
}

internal fun Int.isAsciiPrintable(): Boolean {
    return this in 0x20..0x7E
}

internal fun String.unicodeSubstr(start: Int, end: Int): String {
    val startChar = this.offsetByCodePoints(0, start)
    val endChar = this.offsetByCodePoints(0, end)

    return this.substring(startChar, endChar)
}

internal fun String.unicodeLength(): Int {
    return this.codePointCount(0, this.length)
}