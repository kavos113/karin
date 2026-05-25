package com.github.kavos113.karin.ui.props

import org.junit.jupiter.api.Test
import kotlin.test.assertEquals

class LayoutTest {

    @Test
    fun testLayout_padding_specificIsPrioritizedOverGeneral() {
        val layout = Layout.Default.padding(all = 10f, horizontal = 20f, left = 30f)
        assertEquals(10f, layout.paddingTop)
        assertEquals(10f, layout.paddingBottom)
        assertEquals(30f, layout.paddingLeft)
        assertEquals(20f, layout.paddingRight)
    }

    @Test
    fun testLayout_padding_overwrite() {
        val layout = Layout.Default
            .padding(all = 10f)
            .padding(top = 20f)
        assertEquals(20f, layout.paddingTop)
        assertEquals(10f, layout.paddingBottom)
        assertEquals(10f, layout.paddingLeft)
        assertEquals(10f, layout.paddingRight)
    }

    @Test
    fun testLayout_margin_specificIsPrioritizedOverGeneral() {
        val layout = Layout.Default.margin(all = 10f, horizontal = 20f, left = 30f)
        assertEquals(10f, layout.marginTop)
        assertEquals(10f, layout.marginBottom)
        assertEquals(30f, layout.marginLeft)
        assertEquals(20f, layout.marginRight)
    }

    @Test
    fun testLayout_margin_overwrite() {
        val layout = Layout.Default
            .margin(all = 10f)
            .margin(top = 20f)
        assertEquals(20f, layout.marginTop)
        assertEquals(10f, layout.marginBottom)
        assertEquals(10f, layout.marginLeft)
        assertEquals(10f, layout.marginRight)
    }

    @Test
    fun testLayout_size() {
        val layout = Layout.Default.size(width = 100f, height = 200f)
        assertEquals(100f, layout.width)
        assertEquals(200f, layout.height)
    }
}