#ifndef CORE_SRC_GUI_ACTION_EVENT_POOL_H
#define CORE_SRC_GUI_ACTION_EVENT_POOL_H

#include <cstdint>

#include <any>
#include <functional>
#include <unordered_map>

#include <karin/system/event.h>

namespace karin::gui
{
class ActionEventExecuter
{
public:
    uint32_t addActionEventHandler(const std::function<void(std::any)>& handler);
    void clearActionEvent(uint32_t id);

    void dispatchActionEvent(const ActionEvent& event);

private:
    std::unordered_map<uint32_t, std::function<void(std::any)>> m_actionEventHandlers;
    uint32_t m_nextHandlerId = 1;
};
} // karin::gui

#endif //CORE_SRC_GUI_ACTION_EVENT_POOL_H
