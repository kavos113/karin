package com.github.kavos113.karin.ui.common

import org.junit.jupiter.api.Test
import kotlin.test.assertEquals

class ColorTest {

    @Test
    fun testColor_constructor_int() {
        val color = Color(255, 0, 0, 128)
        assertEquals(1f, color.r)
        assertEquals(0f, color.g)
        assertEquals(0f, color.b)
        assertEquals(128 / 255f, color.a)
    }

    @Test
    fun testColor_constructor_rgbaInt() {
        val color = Color(0xFF000080.toInt())
        assertEquals(1f, color.r)
        assertEquals(0f, color.g)
        assertEquals(0f, color.b)
        assertEquals(128 / 255f, color.a)
    }
}