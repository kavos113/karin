package com.github.kavos113.karin.app

import com.github.kavos113.karin.engine.jni.JniApplicationBridge
import io.mockk.every
import io.mockk.just
import io.mockk.mockkObject
import io.mockk.runs
import io.mockk.unmockkAll
import io.mockk.verify
import org.junit.jupiter.api.AfterEach
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.Test

class KarinTest {

    @BeforeEach
    fun setUp() {
        mockkObject(JniApplicationBridge)
        every {
            JniApplicationBridge.create()
        } returns 123L
        every {
            JniApplicationBridge.run(any())
        } just runs
    }

    @AfterEach
    fun tearDown() {
        unmockkAll()
    }

    @Test
    fun testKarin_run_callsApplicationRun() {
        Karin.run()
        verify {
            JniApplicationBridge.run(123L)
        }
    }
}