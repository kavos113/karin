package com.github.kavos113.karin.engine.memory

import java.lang.ref.Cleaner

internal object NativeResourceManager {
    val cleaner: Cleaner = Cleaner.create()
}