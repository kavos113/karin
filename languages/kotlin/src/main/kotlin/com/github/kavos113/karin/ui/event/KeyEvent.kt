package com.github.kavos113.karin.ui.event

data class KeyEvent(
    val key: Key,
    val physicalKey: Key,
    val modifiers: KeyModifierMask
)
