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

void ApplicationEventDispatcher::dispatchActionEvent(const ActionEvent& event)
{
    if (m_actionEventHandlers.contains(event.actionId))
    {
        m_actionEventHandlers[event.actionId](event.data);
    }
}
} // karin::gui