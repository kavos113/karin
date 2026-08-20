package com.github.kavos113.karin.ui.internal

import org.junit.jupiter.api.Test
import kotlin.test.assertEquals

class UnicodeTest {

    @Test
    fun testUnicodeLength_includeJapaneseText() {
        val str = "Hello, 世界"
        assertEquals(9, str.unicodeLength())
    }

    @Test
    fun testUnicodeLength_includeEmoji() {
        val str = "Hello, 🌍"
        assertEquals(8, str.unicodeLength())
    }

    @Test
    fun testUnicodeLength_allJapaneseText() {
        val str = "こんにちは"
        assertEquals(5, str.unicodeLength())
    }

    @Test
    fun testUnicodeLength_allEmoji() {
        val str = "🌍🌎🌏"
        assertEquals(3, str.unicodeLength())
    }

    @Test
    fun testUnicodeLength_mixedText() {
        val str = "Hello, 世界🌍"
        assertEquals(10, str.unicodeLength())
    }

    @Test
    fun testUnicodeLength_includeSurrogatePairs() {
        val str = "Hello, 𠜎"
        assertEquals(8, str.unicodeLength())
    }

    @Test
    fun testUnicodeLength_includeControlChars() {
        val str = "Hello,\nWorld!"
        assertEquals(13, str.unicodeLength())
    }

    @Test
    fun testUnicodeSubstr_includeJapaneseText() {
        val str = "Hello, 世界"
        assertEquals("Hello", str.unicodeSubstr(0, 5))
        assertEquals("世界", str.unicodeSubstr(7, 9))
    }

    @Test
    fun testUnicodeSubstr_includeEmoji() {
        val str = "Hello, 🌍"
        assertEquals("Hello", str.unicodeSubstr(0, 5))
        assertEquals("🌍", str.unicodeSubstr(7, 8))
    }

    @Test
    fun testUnicodeSubstr_allJapaneseText() {
        val str = "こんにちは"
        assertEquals("こん", str.unicodeSubstr(0, 2))
        assertEquals("にちは", str.unicodeSubstr(2, 5))
    }

    @Test
    fun testUnicodeSubstr_allEmoji() {
        val str = "🌍🌎🌏"
        assertEquals("🌍", str.unicodeSubstr(0, 1))
        assertEquals("🌎", str.unicodeSubstr(1, 2))
        assertEquals("🌏", str.unicodeSubstr(2, 3))
    }

    @Test
    fun testUnicodeSubstr_mixedText() {
        val str = "Hello, 世界🌍だよ"
        assertEquals("Hello", str.unicodeSubstr(0, 5))
        assertEquals("世界", str.unicodeSubstr(7, 9))
        assertEquals("🌍", str.unicodeSubstr(9, 10))
        assertEquals("界🌍だ", str.unicodeSubstr(8, 11))
    }

    @Test
    fun testUnicodeSubstr_includeSurrogatePairs() {
        val str = "Hello, 𠜎"
        assertEquals("Hello", str.unicodeSubstr(0, 5))
        assertEquals("𠜎", str.unicodeSubstr(7, 8))
        assertEquals(" 𠜎", str.unicodeSubstr(6, 8))
    }

    @Test
    fun testUnicodeSubstr_includeControlChars() {
        val str = "Hello,\nWorld!"
        assertEquals("Hello", str.unicodeSubstr(0, 5))
        assertEquals("\nWor", str.unicodeSubstr(6, 10))
        assertEquals("ld!", str.unicodeSubstr(10, 13))
    }

    @Test
    fun testUnicodeSubstr_substrAllRange() {
        val str = "Hello, 世界"
        assertEquals(str, str.unicodeSubstr(0, str.unicodeLength()))
    }

    @Test
    fun testUnicodeInsert_includeJapaneseText() {
        val str = "Hello, 世界"
        assertEquals("Hello, こんにちは世界", str.unicodeInsert(7, "こんにちは"))
    }

    @Test
    fun testUnicodeInsert_includeEmoji() {
        val str = "Hello, 世界"
        assertEquals("Hello, 世界🌍", str.unicodeInsert(9, "🌍"))
    }

    @Test
    fun testUnicodeInsert_includeAfterEmoji() {
        val str = "Hello, 世界🌍だよ"
        assertEquals("Hello, 世界🌍🍋だよ", str.unicodeInsert(10, "🍋"))
    }

    @Test
    fun testUnicodeInsert_includeSurrogatePairs() {
        val str = "Hello, 世界"
        assertEquals("Hello, 世界𠜎", str.unicodeInsert(9, "𠜎"))
    }

    @Test
    fun testUnicodeInsert_insertAfterSurrogatePairs() {
        val str = "Hello, 世界𠜎あああ"
        assertEquals("Hello, 世界𠜎🍋あああ", str.unicodeInsert(10, "🍋"))
    }

    @Test
    fun testUnicodeInsert_insertAtBeginning() {
        val str = "Hello, 世界"
        assertEquals("🍋Hello, 世界", str.unicodeInsert(0, "🍋"))
    }

    @Test
    fun testUnicodeInsert_insertAtEnd() {
        val str = "Hello, 世界"
        assertEquals("Hello, 世界🍋", str.unicodeInsert(9, "🍋"))
    }

    @Test
    fun testUnicodeInsert_insertInMiddle() {
        val str = "Hello, 世界"
        assertEquals("Hello, 🍋世界", str.unicodeInsert(7, "🍋"))
    }

    @Test
    fun testUnicodeInsert_insertMultipleCharacters() {
        val str = "Hello, 世界"
        assertEquals("Hello, 🍋🍋🍋世界", str.unicodeInsert(7, "🍋🍋🍋"))
    }
}