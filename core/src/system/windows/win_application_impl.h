#ifndef SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H
#define SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H

#include <queue>

#include <windows.h>

#include <karin/system/event.h>
#include <karin/system/window.h>

#include <application_impl.h>
#include <action_event_manager.h>
#include <action_event_dispatcher.h>

#define WM_KARIN_ACTION (WM_USER + 1)

namespace karin
{
class WinApplicationImpl : public IApplicationImpl, public IActionEventManager
{
public:
    WinApplicationImpl();
    ~WinApplicationImpl() override = default;

    bool waitEvent(EventPayload& event) override;
    void shutdown() override;

    void pushEvent(const Event& event, WindowID window);

    void setDispatcher(ActionEventDispatcher* dispatcher) override;

    void notifyActionEvent() override;
    void addActionEvent(const ActionEvent& event) override;

    bool m_isRunning = false;

    static constexpr auto CLASS_NAME = L"KarinWindow";

private:
    void createMessageWindow();

    std::queue<EventPayload> m_eventQueue;
    ActionEventDispatcher *m_dispatcher = nullptr;

    HWND m_messageWindow = nullptr;
};
} // karin

#endif //SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H
