#ifndef GUI_EVENT_DISPATCHER_H
#define GUI_EVENT_DISPATCHER_H

#include <cstdint>

#include <any>
#include <unordered_map>
#include <functional>

#include <karin/system/event.h>
#include <karin/gui/view_node.h>

namespace karin::gui
{

class EventDispatcher
{
public:
    explicit EventDispatcher(ViewNode *rootView);

    void dispatchEvent(const Event& event);
    uint32_t addActionEventHandler(const std::function<void(std::any)>& handler);

private:
    void handleMouseMoveEvent(const MouseMoveEvent& event);
    void handleMouseButtonEvent(const MouseButtonEvent& event);
    void handleMouseWheelEvent(const MouseWheelEvent& event) const;
    void handleKeyEvent(const KeyEvent& event) const;
    void handleKeyTypeEvent(const KeyTypeEvent& event) const;

    ViewNode *m_rootView;

    ViewNode *m_hoveredNode = nullptr;
    ViewNode *m_focusNode = nullptr;

    std::unordered_map<uint32_t, std::function<void(std::any)>> m_actionEventHandlers;
    uint32_t m_nextHandlerId = 1;
};

} // karin::gui

#endif //GUI_EVENT_DISPATCHER_H
