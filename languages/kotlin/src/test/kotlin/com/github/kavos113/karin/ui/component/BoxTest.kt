package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.engine.jni.JniViewNodeBridge
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.common.Color
import com.github.kavos113.karin.ui.props.Layout
import com.github.kavos113.karin.ui.props.Style
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

class BoxTest {

    @BeforeEach
    fun setUp() {
        mockkObject(JniContainerNodeBridge)
        mockkObject(JniViewNodeBridge)

        every {
            JniContainerNodeBridge.create()
        } returns 123L
        every {
            JniContainerNodeBridge.addChild(any(), any())
        } just runs

        every {
            JniViewNodeBridge.setBackgroundColor(any(), any(), any(), any(), any())
        } just runs
        every {
            JniViewNodeBridge.setWidth(any(), any())
        } just runs
        every {
            JniViewNodeBridge.setHeight(any(), any())
        } just runs
    }

    @AfterEach
    fun tearDown() {
        unmockkAll()
    }

    @Test
    fun testBox_style_applyStylesToNative() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)
        val builder = object : UiBuilder() {
            override val parentContainer: ContainerNodeHandle = mockContainer
        }

        val style = Style.Default
            .background(color = Color(1f, 0f, 0f, 1f))

        builder.Box(style = style) {}

        verify(atLeast = 1) {
            JniViewNodeBridge.setBackgroundColor(123L, 1f, 0f, 0f, 1f)
        }
    }

    @Test
    fun testBox_styleState_applyStylesToNativeOnChange() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)
        val builder = object : UiBuilder() {
            override val parentContainer: ContainerNodeHandle = mockContainer
        }

        val backgroundColorState = State(Color(1f, 0f, 0f, 1f))
        val styleState = Style.Default
            .background(backgroundColorState)

        builder.Box(style = styleState) {}

        backgroundColorState.value = Color(0f, 1f, 0f, 1f)

        verify(atLeast = 1) {
            JniViewNodeBridge.setBackgroundColor(123L, 0f, 1f, 0f, 1f)
        }
    }

    @Test
    fun testBox_layout_applyLayoutToNative() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)
        val builder = object : UiBuilder() {
            override val parentContainer: ContainerNodeHandle = mockContainer
        }

        val layout = Layout.Default
            .width(100f)
            .height(50f)

        builder.Box(layout = layout) {}

        verify(atLeast = 1) {
            JniViewNodeBridge.setWidth(123L, 100f)
            JniViewNodeBridge.setHeight(123L, 50f)
        }
    }
}