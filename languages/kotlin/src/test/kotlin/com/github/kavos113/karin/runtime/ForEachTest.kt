package com.github.kavos113.karin.runtime

import com.github.kavos113.karin.engine.handle.ContainerNodeHandle
import com.github.kavos113.karin.engine.jni.JniContainerNodeBridge
import com.github.kavos113.karin.engine.jni.JniTextNodeBridge
import com.github.kavos113.karin.engine.jni.JniViewNodeBridge
import com.github.kavos113.karin.ui.UiBuilder
import com.github.kavos113.karin.ui.component.Text
import com.github.kavos113.karin.ui.component.Box
import io.mockk.every
import io.mockk.just
import io.mockk.mockk
import io.mockk.mockkObject
import io.mockk.runs
import io.mockk.unmockkAll
import io.mockk.verify
import org.junit.jupiter.api.AfterEach
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.Test

class ForEachTest {

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
            JniContainerNodeBridge.removeChild(any(), any())
        } just runs
        every {
            JniContainerNodeBridge.insertChild(any(), any(), any())
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
        ) } returns 456L
        every { JniTextNodeBridge.setText(any(), any()) } just runs
    }

    @AfterEach
    fun tearDown() {
        unmockkAll()
    }

    @Test
    fun testForEach_addAllChildren() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)

        val builder = object : UiBuilder() {
            override val parentContainer = mockContainer
            override val viewUpdateRequester = mockContainer
        }

        val list = State(listOf("first", "second", "third"))
        builder.Box {
            ForEach(list) { str ->
                Text(str)
            }
        }

        verify(exactly = 3) {
            JniContainerNodeBridge.addChild(any(), any())
        }
    }

    @Test
    fun testForEach_insertNewNode() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)

        val builder = object : UiBuilder() {
            override val parentContainer = mockContainer
            override val viewUpdateRequester = mockContainer
        }

        val list = State(listOf("first", "second", "third"))
        builder.Box {
            ForEach(list) { str ->
                Text(str)
            }
        }

        list.value += "forth"

        verify(exactly = 3) {
            JniContainerNodeBridge.addChild(any(), any())
        }
        verify(exactly = 1) {
            JniContainerNodeBridge.insertChild(any(), any(), 3)
        }
        verify(exactly = 0) {
            JniContainerNodeBridge.removeChild(any(), any())
        }
    }

    @Test
    fun testForEach_insertNewNodeInsideOfList() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)

        val builder = object : UiBuilder() {
            override val parentContainer = mockContainer
            override val viewUpdateRequester = mockContainer
        }

        val list = State(listOf("first", "second", "third"))
        builder.Box {
            ForEach(list) { str ->
                Text(str)
            }
        }

        list.value = listOf("first", "second", "new!!!!!", "third")

        verify(exactly = 3) {
            JniContainerNodeBridge.addChild(any(), any())
        }
        verify(exactly = 1) {
            JniContainerNodeBridge.insertChild(any(), any(), 2)
        }
        verify(exactly = 0) {
            JniContainerNodeBridge.removeChild(any(), any())
        }
    }

    @Test
    fun testForEach_removeExistingNode() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)

        val builder = object : UiBuilder() {
            override val parentContainer = mockContainer
            override val viewUpdateRequester = mockContainer
        }

        val list = State(listOf("first", "second", "third"))
        builder.Box {
            ForEach(list) { str ->
                Text(str)
            }
        }

        list.value = listOf("first", "third")

        verify(exactly = 3) {
            JniContainerNodeBridge.addChild(any(), any())
        }
        verify(exactly = 0) {
            JniContainerNodeBridge.insertChild(any(), any(), any())
        }
        verify(exactly = 1) {
            JniContainerNodeBridge.removeChild(any(), any())
        }
    }

    @Test
    fun testForEach_moveExistingNode() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)

        val builder = object : UiBuilder() {
            override val parentContainer = mockContainer
            override val viewUpdateRequester = mockContainer
        }

        val list = State(listOf("first", "second", "third"))
        builder.Box {
            ForEach(list) { str ->
                Text(str)
            }
        }

        list.value = listOf("first", "third", "second")

        verify(exactly = 3) {
            JniContainerNodeBridge.addChild(any(), any())
        }
        verify(exactly = 1) {
            JniContainerNodeBridge.insertChild(any(), any(), 1)
            // don't call insertChild(any(), any(), 2) because if insert "third", children is already ["first", "third", "second"]
        }
        verify(exactly = 1) {
            JniContainerNodeBridge.removeChild(any(), any())
        }
    }

    @Test
    fun testForEach_multipleChanges() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)

        val builder = object : UiBuilder() {
            override val parentContainer = mockContainer
            override val viewUpdateRequester = mockContainer
        }

        val list = State(listOf("first", "second", "third"))
        builder.Box {
            ForEach(list) { str ->
                Text(str)
            }
        }

        list.value = listOf("kawatta", "second", "first", "new node", "very new node")

        verify(exactly = 3) {
            JniContainerNodeBridge.addChild(any(), any())
        }
        verify(exactly = 1) {
            // order
            //   0: ["first", "second"] (remove)
            //   1: ["kawatta", "first", "second"] (insert 0)
            //   2: ["kawatta", "second", "first"] (remove & insert 1)
            //   3: ["kawatta", "second", "first", "new node"] (insert 3)
            //   4: ["kawatta", "second", "first", "new node", "very new node"] (insert 4)

            JniContainerNodeBridge.insertChild(any(), any(), 0)
            JniContainerNodeBridge.insertChild(any(), any(), 1)
            JniContainerNodeBridge.insertChild(any(), any(), 3)
            JniContainerNodeBridge.insertChild(any(), any(), 4)
        }
        verify(exactly = 2) {
            JniContainerNodeBridge.removeChild(any(), any())
        }
    }

    @Test
    fun testForEach_multipleComponentPerItem() {
        val mockContainer = mockk<ContainerNodeHandle>(relaxed = true)

        val builder = object : UiBuilder() {
            override val parentContainer = mockContainer
            override val viewUpdateRequester = mockContainer
        }

        val list = State(listOf(3, 1, 5, 2, 4, 6))
        builder.Box {
            ForEach(list) { i ->
                Text("hello")
                Text("number is $i")
            }
        }

        list.value += 7

        verify(exactly = 12) {
            JniContainerNodeBridge.addChild(any(), any())
        }
        verify(exactly = 1) {
            JniContainerNodeBridge.insertChild(any(), any(), 12)
            JniContainerNodeBridge.insertChild(any(), any(), 13)
        }
        verify(exactly = 0) {
            JniContainerNodeBridge.removeChild(any(), any())
        }
    }
}
