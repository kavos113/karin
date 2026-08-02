package com.github.kavos113.karin.engine.handle

interface ViewUpdateRequester {
    fun requestRedraw()
    fun requestRelayout()
}