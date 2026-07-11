package com.github.kavos113.karin.runtime

class State<T>(initialValue: T) {
    private val lock = Any()
    private val listeners = mutableListOf<(T) -> Unit>()

    var value: T = initialValue
        set(value) {
            if (field == value) {
                return
            }

            val snapshot = synchronized(lock) {
                field = value
                listeners.toList()
            }
            snapshot.forEach { it(value) }
        }

    fun onChange(listener: (T) -> Unit): () -> Unit {
        val current = synchronized(lock) {
            listeners.add(listener)
            value
        }
        listener(current)

        return {
            synchronized(lock) {
                listeners.remove(listener)
            }
        }
    }

    fun <R> map(transform: (T) -> R): State<R> {
        val mappedState = State(transform(value))
        onChange { mappedState.value = transform(it) }
        return mappedState
    }
}