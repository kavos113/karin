#ifndef CORE_SRC_SYSTEM_ACTION_EVENT_DISPATCHER_H
#define CORE_SRC_SYSTEM_ACTION_EVENT_DISPATCHER_H

#include <mutex>
#include <queue>
#include <any>
#include <functional>
#include <variant>

#include <karin/system/event.h>
#include "action_event_manager.h"

namespace karin
{
class ActionEventDispatcher
{
public:
    ActionEventDispatcher(IActionEventManager *target);
    ~ActionEventDispatcher();

    void sendAction(uint32_t id, const std::any& data);
    void sendTask(const std::function<void()>& task);

    /**
     * send events in m_pendingActions to main message loop event queue.
     * this MUST be called from main thread (expect: from main message loop)
     */
    void handlePostActionEvent();

private:
    using PendingItem = std::variant<ActionEvent, TaskEvent>;

    std::mutex m_mtx;
    std::queue<PendingItem> m_pendingActions;
    bool m_isAlertPending = false;

    IActionEventManager *m_target;
};
} // karin

#endif //CORE_SRC_SYSTEM_ACTION_EVENT_DISPATCHER_H
