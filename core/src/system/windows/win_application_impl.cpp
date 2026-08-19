#include "win_application_impl.h"

#include "win_window_class_registry.h"
#include "win_window_impl.h"
#include "win_context.h"
#include "win_message_handler.h"

namespace karin
{
WinApplicationImpl::WinApplicationImpl()
{
    WNDCLASSEX wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = 0,
        .lpfnWndProc = windowProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = GetModuleHandle(nullptr),
        .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
        .lpszMenuName = nullptr,
        .hIconSm = LoadIcon(nullptr, IDI_APPLICATION)
    };

    WinContext::instance().windowClassRegistry().registerClass(wc, CLASS_NAME);

    createMessageWindow();
}

void WinApplicationImpl::pushEvent(const Event& event, WindowID window)
{
    EventPayload payload{
        .windowId = window,
        .event = event
    };
    m_eventQueue.push(payload);
}

bool WinApplicationImpl::waitEvent(EventPayload &event)
{
    if (!m_isRunning)
        m_isRunning = true;

    if (!m_eventQueue.empty())
    {
        event = m_eventQueue.front();
        m_eventQueue.pop();
        return true;
    }

    while (m_eventQueue.empty())
    {
        MSG msg;
        if (GetMessage(&msg, nullptr, 0, 0) <= 0)
        {
            m_isRunning = false;
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    event = m_eventQueue.front();
    m_eventQueue.pop();

    return true;
}

void WinApplicationImpl::shutdown()
{
    if (!m_isRunning)
        return;

    PostQuitMessage(0);
}

void WinApplicationImpl::setDispatcher(ActionEventDispatcher* dispatcher)
{
    m_dispatcher = dispatcher;
}

void WinApplicationImpl::notifyActionEvent()
{
    PostMessage(m_messageWindow, WM_KARIN_ACTION, 0, 0);
}

void WinApplicationImpl::addActionEvent(const ActionEvent& event)
{
    pushEvent(event, 0);
}
} // karin