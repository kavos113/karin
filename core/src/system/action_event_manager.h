#ifndef CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H
#define CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H

#include <karin/system/event.h>

namespace karin
{
class IActionEventManager
{
public:
    virtual ~IActionEventManager() = default;

    virtual void triggerActionEvent() = 0;
    virtual void addActionEvent(const ActionEvent& event) = 0;
};
} // karin

#endif //CORE_SRC_SYSTEM_ACTION_EVENT_MANAGER_H
