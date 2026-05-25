package com.github.kavos113.karin.runtime

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.engine.jni.JniTextNodeBridge
import com.github.kavos113.karin.engine.jni.JniViewNodeBridge
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.component.Text
import io.mockk.every
import io.mockk.just
import io.mockk.mockk
import io.mockk.mockkObject
import io.mockk.runs
import io.mockk.unmockkAll
import io.mockk.verify
import org.junit.jupiter.api.AfterEach
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.Test

class WatchTest {

    @BeforeEach
    fun setUp() {
        mockkObject(JniContainerNodeBridge)
        every {
            JniContainerNodeBridge.create()
        } returns 123L
        every {
            JniContainerNodeBridge.addChild(any(), any())
        } just runs
        every {
            JniContainerNodeBridge.clearChildren(any())
        } just runs

        mockkObject(JniViewNodeBridge)
        every {
            JniViewNodeBridge.requestRelayout(any())
        } just runs


        mockkObject(JniTextNodeBridge)
        every { JniTextNodeBridge.create(
            text = any(),
            fontFamily = any(),
            fontStyle = any(),
            fontStretch = any(),
            fontWeight = any(),
            fontSize = any(),
            locale = any(),
            underline = any(),
            strikethrough = any(),
            horizontalAlignment = any(),
            verticalAlignment = any(),
            lineSpacing = any(),
            baseline = any(),
            lineSpacingMode = any(),
            trimming = any(),
            wrapping = any(),
            flowDirection = any(),
            readingDirection = any(),
            r = any(),
            g = any(),
            b = any(),
            a = any()
        ) } returns 123L
        every { JniTextNodeBridge.setText(any(), any()) } just runs
    }

    @AfterEach
    fun tearDown() {
        unmockkAll()
    }

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
            JniContainerNodeBridge.clearChildren(123L)
            JniViewNodeBridge.requestRelayout(123L)
        }
    }
}