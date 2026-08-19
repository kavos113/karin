#ifndef CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H
#define CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H

#include <karin/system/event.h>

#include "action_event_dispatcher.h"

namespace karin
{
class IActionEventManager
{
public:
    virtual ~IActionEventManager() = default;

    virtual void setDispatcher(ActionEventDispatcher* dispatcher) = 0;

    virtual void notifyActionEvent() = 0;
    virtual void addActionEvent(const ActionEvent& event) = 0;
};
} // karin

#endif //CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H
