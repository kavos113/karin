#ifndef GUI_EVENT_DISPATCHER_H
#define GUI_EVENT_DISPATCHER_H

#include <karin/system/event.h>
#include <karin/gui/view_node.h>

namespace karin::gui
{

class EventDispatcher
{
public:
    explicit EventDispatcher(ViewNode *rootView);

    void dispatchEvent(const Event& event);

private:
    void handleMouseMoveEvent(const MouseMoveEvent& event);
    void handleMouseButtonEvent(const MouseButtonEvent& event);
    void handleMouseWheelEvent(const MouseWheelEvent& event) const;
    void handleKeyEvent(const KeyEvent& event) const;
    void handleKeyTypeEvent(const KeyTypeEvent& event) const;

    ViewNode *m_rootView;

    ViewNode *m_hoveredNode = nullptr;
    ViewNode *m_focusNode = nullptr;
};

} // karin::gui

#endif //GUI_EVENT_DISPATCHER_H
