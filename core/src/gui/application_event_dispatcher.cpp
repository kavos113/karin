#include "application_event_dispatcher.h"

namespace karin::gui
{
uint32_t ApplicationEventDispatcher::addActionEventHandler(const std::function<void(std::any)>& handler)
{
    uint32_t id = m_nextHandlerId;
    m_actionEventHandlers[id] = handler;

    m_nextHandlerId++;
    return id;
}

void ApplicationEventDispatcher::clearActionEvent(uint32_t id)
{
    if (m_actionEventHandlers.contains(id))
    {
        m_actionEventHandlers.erase(id);
    }
}

void ApplicationEventDispatcher::dispatchEvent(const Event& event)
{
    std::visit(
        [this]<typename T0>(const T0& e)
        {
            using T = std::decay_t<T0>;

            if constexpr (std::is_same_v<T, ActionEvent>)
            {
                if (m_actionEventHandlers.contains(e.actionId))
                {
                    auto fun = m_actionEventHandlers[e.actionId];
                    fun(e.data);
                }
            }
            else if constexpr (std::is_same_v<T, TaskEvent>)
            {
                auto fun = e.task;
                fun();
            }
        },
        event
    );
}
} // karin::gui