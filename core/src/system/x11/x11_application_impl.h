#ifndef SYSTEM_X11_X11_APPLICATION_IMPL_H
#define SYSTEM_X11_X11_APPLICATION_IMPL_H

#include <map>
#include <queue>

#include <poll.h>
#include <X11/Xlib.h>

#include <x11/window.h>
#include <application_impl.h>
#include <action_event_manager.h>
#include <action_event_dispatcher.h>

namespace karin
{
class X11WindowImpl;

class X11ApplicationImpl : public IApplicationImpl, IActionEventManager
{
public:
    X11ApplicationImpl();
    ~X11ApplicationImpl() override;

    void addWindow(XlibWindow window, X11WindowImpl* impl);

    void shutdown() override;
    bool waitEvent(EventPayload& event) override;
    void pushEvent(const Event& event, WindowID window);

    void setDispatcher(ActionEventDispatcher* dispatcher) override;
    void notifyActionEvent() override;
    void addActionEvent(const ActionEvent& event) override;

private:
    static int errorHandler(Display* display, XErrorEvent* error);

    std::map<XlibWindow, X11WindowImpl*> m_windows;

    bool m_running = true;
    std::queue<EventPayload> m_eventQueue;

    struct pollfd m_pollfds[2] = {};

    ActionEventDispatcher *m_dispatcher;
};
} // karin

#endif //SYSTEM_X11_X11_APPLICATION_IMPL_H
