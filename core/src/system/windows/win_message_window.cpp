#include "win_message_window.h"

#include <karin/system/window.h>
#include "action_event_dispatcher.h"

namespace karin
{
WinMessageWindow::WinMessageWindow(WinApplicationImpl* appImpl)
    : m_appImpl(appImpl)
{
    m_hwnd = CreateWindowEx(
        0,
        WinApplicationImpl::CLASS_NAME,
        L"karin_message_window",
        0,
        0, 0, 0, 0,
        HWND_MESSAGE,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );
}

WinMessageWindow::~WinMessageWindow() = default;

void WinMessageWindow::setDispatcher(ActionEventDispatcher* dispatcher)
{
    m_dispatcher = dispatcher;
}

void WinMessageWindow::notifyActionEvent()
{
    PostMessage(m_hwnd, WM_KARIN_ACTION, 0, 0);
}

void WinMessageWindow::addActionEvent(const ActionEvent& event)
{
    m_appImpl->pushEvent(event, WINDOW_ID_NONE);
}

LRESULT WinMessageWindow::handleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_KARIN_ACTION)
    {
        m_dispatcher->handlePostActionEvent();
        return 0;
    }

    return DefWindowProc(m_hwnd, message, wParam, lParam);
}

void WinMessageWindow::setHwnd(HWND hwnd)
{
    m_hwnd = hwnd;
}
} // karin