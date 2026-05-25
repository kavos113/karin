package com.github.kavos113.karin.app

import com.github.kavos113.karin.engine.jni.JniApplicationBridge
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.engine.jni.JniWindowBridge
import io.mockk.every
import io.mockk.just
import io.mockk.mockkObject
import io.mockk.runs
import io.mockk.unmockkAll
import io.mockk.verify
import org.junit.jupiter.api.AfterEach
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.Test

class WindowTest {

    @BeforeEach
    fun setUp() {
        mockkObject(JniApplicationBridge)
        every {
            JniApplicationBridge.create()
        } returns 123L
        every {
            JniApplicationBridge.createWindow(any(), any(), any(), any(), any(), any())
        } returns 456L
    }

    @AfterEach
    fun tearDown() {
        unmockkAll()
    }

    @Test
    fun testWindow_creation_callsCreateWindow() {
        val window = Window(
            title = "Test Window",
            x = 100,
            y = 100,
            width = 800,
            height = 600,
        )
        verify {
            JniApplicationBridge.createWindow(
                123L,
                "Test Window",
                100,
                100,
                800,
                600
            )
        }
    }

    @Test
    fun testWindow_setContent_callsSetRootView() {
        mockkObject(JniWindowBridge)
        every {
            JniWindowBridge.setRootView(any(), any())
        } just runs

        mockkObject(JniContainerNodeBridge)
        every {
            JniContainerNodeBridge.create()
        } returns 789L

        val window = Window(
            title = "Test Window",
            x = 100,
            y = 100,
            width = 800,
            height = 600,
        )
        window.setContent { }

        verify {
            JniContainerNodeBridge.create()
            JniWindowBridge.setRootView(456L, 789L)
        }
    }
}