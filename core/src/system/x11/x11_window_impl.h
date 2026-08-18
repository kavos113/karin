#ifndef SYSTEM_X11_X11_WINDOW_IMPL_H
#define SYSTEM_X11_X11_WINDOW_IMPL_H
#include <mutex>
#include <string>
#include <vector>
#include <X11/Xlib.h>

#include <karin/system/window.h>
#include <x11/window.h>
#include <window_impl.h>
#include <action_event_manager.h>
#include <action_event_dispatcher.h>

#include "x11_application_impl.h"

namespace karin
{
class X11WindowImpl : public IWindowImpl, public IActionEventManager
{
public:
    X11WindowImpl(
        const std::string& title,
        int x,
        int y,
        int width,
        int height,
        X11ApplicationImpl* appImpl,
        WindowID owner
    );
    ~X11WindowImpl() override;

    void show() override;
    void hide() override;
    void minimize() override;
    void maximize() override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;
    void setRect(int x, int y, int width, int height) override;

    void addPaintCallback(std::function<bool()> onPaint) override;
    void addResizeCallback(std::function<void(Size)> onResize) override;
    void addStartResizeCallback(std::function<void()> onStartResize) override;
    void addFinishResizeCallback(std::function<void()> onFinishResize) override;

    void invalidate() override;

    void setDispatcher(ActionEventDispatcher* dispatcher) override;
    void notifyActionEvent() override;
    void addActionEvent(const ActionEvent& event) override;
    void addTaskEvent(const TaskEvent& event) override;

    [[nodiscard]] Window::NativeHandle handle() const override;

    void handleEvent(const XEvent& event);

private:
    void applyStatus();

    XlibWindow m_window;
    GC m_gc;
    XIM m_xim;
    XIC m_xic;
    Atom m_actionEventAtom;

    std::vector<std::function<bool()>> m_paintCallbacks;
    std::vector<std::function<void(Size)>> m_resizeCallbacks;
    std::vector<std::function<void()>> m_startResizeCallbacks;
    std::vector<std::function<void()>> m_finishResizeCallbacks;

    std::function<void()> m_onClose;
    std::function<void()> m_onStartMainLoop;

    enum class X11ShowStatus
    {
        Maximize,
        Minimize,
        NoStatus
    };

    X11ShowStatus m_status = X11ShowStatus::NoStatus;
    std::once_flag m_applyStatusFlag;

    X11ApplicationImpl* m_appImpl = nullptr;
    WindowID m_id;

    ActionEventDispatcher *m_eventDispatcher;
};
} // karin

#endif //SYSTEM_X11_X11_WINDOW_IMPL_H
