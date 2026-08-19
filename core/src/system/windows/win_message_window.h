#ifndef CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_WINDOW_H
#define CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_WINDOW_H

#include <windows.h>

#include <karin/system/event.h>

#include <action_event_manager.h>

#include "win_message_handler.h"
#include "win_application_impl.h"

#define WM_KARIN_ACTION (WM_USER + 1)

namespace karin
{
/**
 * WinMessageWindow is only used to handle ActionEvent, TaskEvent
 * (no view)
 */
class WinMessageWindow : public IWinMessageHandler, public IActionEventManager
{
public:
    WinMessageWindow(WinApplicationImpl *appImpl);
    ~WinMessageWindow() override;

    void setDispatcher(ActionEventDispatcher* dispatcher) override;
    void notifyActionEvent() override;
    void addActionEvent(const ActionEvent& event) override;
    void addTaskEvent(const TaskEvent& event) override;

    LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
    void setHwnd(HWND hwnd) override;

private:
    WinApplicationImpl *m_appImpl;
    ActionEventDispatcher *m_dispatcher = nullptr;

    HWND m_hwnd;
};
} // karin

#endif //CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_WINDOW_H
