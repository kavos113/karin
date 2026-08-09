package com.github.kavos113.karin.ui.event

enum class KeyModifier(val value: Int) {
    Shift(1),
    Control(2),
    Alt(4),
    Super(8);

    infix fun or(other: KeyModifier): KeyModifierMask = KeyModifierMask(this.value or other.value)
    infix fun or(other: KeyModifierMask): KeyModifierMask = KeyModifierMask(this.value or other.value)
}

@JvmInline
value class KeyModifierMask(val value: Int = 0) {
    infix fun or(other: KeyModifier): KeyModifierMask = KeyModifierMask(this.value or other.value)
    infix fun or(other: KeyModifierMask): KeyModifierMask = KeyModifierMask(this.value or other.value)

    infix fun and(other: KeyModifierMask): KeyModifierMask = KeyModifierMask(this.value and other.value)

    operator fun contains(m: KeyModifier): Boolean {
        return (this.value and m.value) != 0
    }
}