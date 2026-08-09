package com.github.kavos113.karin.ui.event

enum class KeyEventType(val value: Int) {
    KeyPress(0),
    KeyRelease(1);

    companion object {
        fun fromInt(value: Int): KeyEventType {
            return entries.find { it.value == value } ?: KeyPress
        }
    }
}