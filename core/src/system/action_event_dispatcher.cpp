#include "action_event_dispatcher.h"

namespace karin
{
ActionEventDispatcher::ActionEventDispatcher(IActionEventManager* target)
    : m_target(target)
{
}

ActionEventDispatcher::~ActionEventDispatcher() = default;

void ActionEventDispatcher::sendAction(uint32_t id, const std::any& data)
{
    bool shouldPost = false;

    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_pendingActions.emplace(ActionEvent(id, data));

        if (!m_isAlertPending)
        {
            m_isAlertPending = true;
            shouldPost = true;
        }
    }

    if (shouldPost)
    {
        m_target->notifyActionEvent();
    }
}

void ActionEventDispatcher::sendTask(const std::function<void()>& task)
{
    bool shouldPost = false;

    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_pendingActions.emplace(TaskEvent(task));

        if (!m_isAlertPending)
        {
            m_isAlertPending = true;
            shouldPost = true;
        }
    }

    if (shouldPost)
    {
        m_target->notifyActionEvent();
    }
}

void ActionEventDispatcher::handlePostActionEvent()
{
    std::queue<PendingItem> snapshot;

    {
        std::lock_guard<std::mutex> lock(m_mtx);
        std::swap(m_pendingActions, snapshot);
        m_isAlertPending = false;
    }

    while (!snapshot.empty())
    {
        auto item = snapshot.front();
        snapshot.pop();

        if (std::holds_alternative<TaskEvent>(item))
        {
            m_target->addTaskEvent(std::get<TaskEvent>(item));
        }
        else
        {
            m_target->addActionEvent(std::get<ActionEvent>(item));
        }
    }
}
} // karin