package com.github.kavos113.karin.ui.component

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.jni.JniTextNodeBridge
import com.github.kavos113.karin.runtime.State
import com.github.kavos113.karin.ui.UiBuilder
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

class TextTest {

    @BeforeEach
    fun setUp() {
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
    fun testText_textState_setTextOnChange() {
        val mockParentContainer = mockk<ContainerNodeHandle>(relaxed = true)
        val builder = object : UiBuilder() {
            override val parentContainer: ContainerNodeHandle = mockParentContainer
        }

        val textState = State("Initial Text")
        builder.Text(text = textState)

        textState.value = "Updated Text"

        verify(exactly = 1) {
            JniTextNodeBridge.setText(123L, "Updated Text")
        }
    }
}