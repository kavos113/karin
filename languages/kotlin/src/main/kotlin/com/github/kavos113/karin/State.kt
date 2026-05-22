package com.github.kavos113.karin

class State<T>(initialValue: T) {
    private val listeners = mutableListOf<(T) -> Unit>()

    var value: T = initialValue
        set(value) = synchronized(this) {
            field = value
            listeners.forEach { it(value) }
        }

    fun onChange(listener: (T) -> Unit) {
        listeners.add(listener)
        listener(value)
    }

    fun <R> map(transform: (T) -> R): State<R> {
        val mappedState = State(transform(value))
        onChange { mappedState.value = transform(it) }
        return mappedState
    }
}