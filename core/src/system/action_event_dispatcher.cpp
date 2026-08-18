#include "action_event_dispatcher.h"

namespace karin
{
ActionEventDispatcher::ActionEventDispatcher(IActionEventManager* target)
    : m_target(target)
{
}

ActionEventDispatcher::~ActionEventDispatcher() = default;

void ActionEventDispatcher::sendActionEvent(uint32_t id, const std::any& data)
{
    bool shouldPost = false;

    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_pendingActions.push(ActionEvent(id, data));

        if (!m_isAlertPending)
        {
            m_isAlertPending = true;
            shouldPost = true;
        }

        if (shouldPost)
        {
            m_target->triggerActionEvent();
        }
    }
}

void ActionEventDispatcher::handlePostActionEvent()
{
    std::queue<ActionEvent> snapshot;

    {
        std::lock_guard<std::mutex> lock(m_mtx);
        std::swap(m_pendingActions, snapshot);
        m_isAlertPending = false;
    }

    while (!snapshot.empty())
    {
        ActionEvent e = snapshot.front();
        snapshot.pop();

        m_target->addActionEvent(e);
    }
}
} // karin