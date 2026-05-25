package com.github.kavos113.karin.runtime

import org.junit.jupiter.api.Test
import kotlin.test.assertEquals

class StateTest {

    @Test
    fun testState_onChange_successfullyNotifiesListener() {
        val state = State(0)
        var notifiedValue: Int? = null

        state.onChange { notifiedValue = it }
        state.value = 42

        assertEquals(42, notifiedValue)
    }

    @Test
    fun testState_onChange_returnsUnsubscribeFunction() {
        val state = State(0)
        var notifiedValue: Int? = null

        val unsubscribe = state.onChange { notifiedValue = it }
        state.value = 42
        assertEquals(42, notifiedValue)

        unsubscribe()
        state.value = 100
        assertEquals(42, notifiedValue) // Should not change after unsubscribe
    }

    @Test
    fun testState_map_createsMappedState() {
        val state = State(2)
        val mappedState = state.map { it * 2 }

        assertEquals(4, mappedState.value)

        state.value = 3
        assertEquals(6, mappedState.value)
    }

    @Test
    fun testState_map_transformToDifferentType() {
        val state = State(2)
        val mappedState = state.map { "Value: $it" }

        assertEquals("Value: 2", mappedState.value)

        state.value = 5
        assertEquals("Value: 5", mappedState.value)
    }
}