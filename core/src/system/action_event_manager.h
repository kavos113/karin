#ifndef CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H
#define CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H

#include <karin/system/event.h>

namespace karin
{
class ActionEventDispatcher;

class IActionEventManager
{
public:
    virtual ~IActionEventManager() = default;

    virtual void setDispatcher(ActionEventDispatcher *dispatcher) = 0;

    virtual void notifyActionEvent() = 0;
    virtual void addActionEvent(const ActionEvent& event) = 0;
    virtual void addTaskEvent(const TaskEvent& event) = 0;
};
} // karin

#endif //CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H
