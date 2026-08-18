#ifndef CORE_SRC_SYSTEM_ACTION_EVENT_DISPATCHER_H
#define CORE_SRC_SYSTEM_ACTION_EVENT_DISPATCHER_H

#include <mutex>
#include <queue>
#include <any>

#include <karin/system/event.h>
#include "action_event_manager.h"

namespace karin
{
class ActionEventDispatcher
{
public:
    ActionEventDispatcher(IActionEventManager *target);
    ~ActionEventDispatcher();

    void sendActionEvent(uint32_t id, const std::any& data);

    /**
     * send events in m_pendingActions to main message loop event queue.
     * this MUST be called from main thread (expect: from main message loop)
     */
    void handlePostActionEvent();

private:
    std::mutex m_mtx;
    std::queue<ActionEvent> m_pendingActions;
    bool m_isAlertPending = false;

    IActionEventManager *m_target;
};
} // karin

#endif //CORE_SRC_SYSTEM_ACTION_EVENT_DISPATCHER_H
