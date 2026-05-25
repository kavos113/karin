package com.github.kavos113.karin.runtime

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.component.Text
import io.mockk.mockk
import io.mockk.verify
import org.junit.jupiter.api.Test

class WatchTest {

    @Test
    fun testWatch_clearChildrenOnStateChange() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)

        val builder = object : UiBuilder() {
            override val parentContainer = mockContainer
        }

        val state = State("initial")
        builder.Watch(state) { value ->
            Text(value)
        }

        state.value = "updated"

        verify(atLeast = 1) {
            mockContainer.clearChildren()
            mockContainer.requestRelayout()
        }
    }
}