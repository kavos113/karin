#ifndef SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H
#define SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H

#include <queue>

#include <windows.h>

#include <karin/system/event.h>
#include <karin/system/window.h>

#include <application_impl.h>
#include <action_event_manager.h>

namespace karin
{
class WinMessageWindow;

class WinApplicationImpl : public IApplicationImpl
{
public:
    WinApplicationImpl();
    ~WinApplicationImpl() override = default;

    bool waitEvent(EventPayload& event) override;
    void shutdown() override;

    void pushEvent(const Event& event, WindowID window);

    IActionEventManager *getActionEventManager() const;

    bool m_isRunning = false;

    static constexpr auto CLASS_NAME = L"KarinWindow";

private:
    std::queue<EventPayload> m_eventQueue;
    ActionEventDispatcher *m_dispatcher = nullptr;

    std::unique_ptr<WinMessageWindow> m_messageWindow;
};
} // karin

#endif //SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H
