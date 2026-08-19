#include "action_event_executer.h"

namespace karin::gui
{
uint32_t ActionEventExecuter::addActionEventHandler(const std::function<void(std::any)>& handler)
{
    uint32_t id = m_nextHandlerId;
    m_actionEventHandlers[id] = handler;

    m_nextHandlerId++;
    return id;
}

void ActionEventExecuter::clearActionEvent(uint32_t id)
{
    if (m_actionEventHandlers.contains(id))
    {
        m_actionEventHandlers.erase(id);
    }
}

void ActionEventExecuter::dispatchActionEvent(const ActionEvent& event)
{
    if (m_actionEventHandlers.contains(event.actionId))
    {
        m_actionEventHandlers[event.actionId](event.data);
    }
}
} // karin::gui