package com.github.kavos113.karin.engine.handle

import com.github.kavos113.karin.engine.jni.JniViewNodeBridge
import com.github.kavos113.karin.engine.memory.NativeResourceManager
import java.lang.ref.Cleaner

internal open class ViewNodeHandle(ptr: Long) {
    private var internalPtr = ptr
    private val cleanable: Cleaner.Cleanable = NativeResourceManager.cleaner.register(this, CleanupTask(ptr))

    private var onClick: (() -> Unit)? = null

    val ptr: Long
        get() {
            check(internalPtr != 0L) {
                "ownership of this handle already transferred to C++ side (or destroyed), cannot access it anymore"
            }
            return internalPtr
        }

    fun transferOwnership(): Long {
        val currentPtr = ptr
        internalPtr = 0L
        cleanable.clean()
        return currentPtr
    }

    fun setOnClickListener(listener: () -> Unit) {
        onClick = listener
        JniViewNodeBridge.setClickListener(ptr, this)
    }

    @JvmName("dispatchClickEvent")
    internal fun dispatchClickEvent() {
        onClick?.invoke()
    }

    private class CleanupTask(private val ptr: Long) : Runnable {
        override fun run() {
            if (ptr != 0L) {
                JniViewNodeBridge.destroy(ptr)
            }
        }
    }
}