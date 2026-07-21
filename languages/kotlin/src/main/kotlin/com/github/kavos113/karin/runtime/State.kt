package com.github.kavos113.karin.runtime

class State<T>(initialValue: T) {
    private val lock = Any()
    private val listeners = mutableListOf<(T) -> Unit>()

    var value: T = initialValue
        get() {
            StateTracker.currentDependencies.get()?.add(this)

            return field
        }
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

fun <R> computedState(block: () -> R): State<R> {
    val dependencies = mutableSetOf<State<*>>()

    // track state in block
    StateTracker.currentDependencies.set(dependencies)
    val initialValue = block() // TODO: if branched in block and registered partial states?
    StateTracker.currentDependencies.remove()

    val resultState = State(initialValue)

    dependencies.forEach { dep ->
        dep.onChange {
            resultState.value = block()
        }
    }

    return resultState
}

private object StateTracker {
    val currentDependencies = ThreadLocal<MutableSet<State<*>>>()
}