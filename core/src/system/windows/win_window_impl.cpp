#include "win_window_impl.h"

#include "win_event.h"
#include "win_window_class_registry.h"

#include <utils/string.h>

#include "action_event_dispatcher.h"

namespace karin
{
WinWindowImpl::WinWindowImpl(
    const std::string& title,
    const int x,
    const int y,
    const int width,
    const int height,
    WinApplicationImpl* appImpl,
    WindowID owner
)
    : m_appImpl(appImpl),
      m_owner(owner)
{
    std::wstring wtitle = toWString(title);

    RECT rect = { 0, 0, width, height };
    AdjustWindowRectEx(
        &rect,
        WS_OVERLAPPEDWINDOW,
        FALSE,
        0
    );

    m_hwnd = CreateWindowEx(
        0,
        WinApplicationImpl::CLASS_NAME,
        wtitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        x, y,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        static_cast<IWinMessageHandler*>(this)
    );
    if (!m_hwnd)
    {
        throw std::runtime_error("Failed to create window");
    }
}

WinWindowImpl::~WinWindowImpl() = default;

LRESULT WinWindowImpl::handleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    std::optional<Event> event = translateWinEvent(message, wParam, lParam);
    if (event.has_value())
    {
        m_appImpl->pushEvent(*event, m_owner);
    }

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
        DestroyWindow(m_hwnd);
        return 0;

    case WM_PAINT:
        ValidateRect(m_hwnd, nullptr);
        for (const auto& callback : m_paintCallbacks)
        {
            if (callback())
            {
                break;
            }
        }
        return 0;

    case WM_SIZE:
        {
            Size newSize(LOWORD(lParam), HIWORD(lParam));
            for (const auto& callback : m_resizeCallbacks)
            {
                callback(newSize);
            }
        }
        return 0;

    case WM_ENTERSIZEMOVE:
        for (const auto& callback : m_startResizeCallbacks)
        {
            callback();
        }
        return 0;

    case WM_EXITSIZEMOVE:
        for (const auto& callback : m_finishResizeCallbacks)
        {
            callback();
        }
        return 0;


    default:
        if (event.has_value())
        {
            return 0;
        }
    }

    return DefWindowProc(m_hwnd, message, wParam, lParam);
}

void WinWindowImpl::setHwnd(HWND hwnd)
{
    m_hwnd = hwnd;
}

void WinWindowImpl::show()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
    }
}

void WinWindowImpl::hide()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_HIDE);
    }
}

void WinWindowImpl::minimize()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_MINIMIZE);
    }

    m_appImpl->pushEvent(WindowEvent(WindowEvent::Type::Minimize), m_owner);
}

void WinWindowImpl::maximize()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_MAXIMIZE);
    }

    m_appImpl->pushEvent(WindowEvent(WindowEvent::Type::Maximize), m_owner);
}

void WinWindowImpl::setPosition(int x, int y)
{
    SetWindowPos(
        m_hwnd,
        nullptr,
        x, y,
        0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
    );
}

void WinWindowImpl::setSize(int width, int height)
{
    SetWindowPos(
        m_hwnd,
        nullptr,
        0, 0,
        width, height,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
    );
}

void WinWindowImpl::setRect(int x, int y, int width, int height)
{
    SetWindowPos(
        m_hwnd,
        nullptr,
        x, y,
        width, height,
        SWP_NOZORDER | SWP_NOACTIVATE
    );
}

Window::NativeHandle WinWindowImpl::handle() const
{
    return Window::NativeHandle{
        .hwnd = m_hwnd,
        .hinstance = GetModuleHandle(nullptr)
    };
}

void WinWindowImpl::addPaintCallback(std::function<bool()> onPaint)
{
    m_paintCallbacks.push_back(std::move(onPaint));
}

void WinWindowImpl::addResizeCallback(std::function<void(Size)> onResize)
{
    m_resizeCallbacks.push_back(std::move(onResize));
}

void WinWindowImpl::addStartResizeCallback(std::function<void()> onStartResize)
{
    m_startResizeCallbacks.push_back(std::move(onStartResize));
}

void WinWindowImpl::addFinishResizeCallback(std::function<void()> onFinishResize)
{
    m_finishResizeCallbacks.push_back(std::move(onFinishResize));
}

void WinWindowImpl::invalidate()
{
    if (m_hwnd)
    {
        InvalidateRect(m_hwnd, nullptr, FALSE);
    }
}
} // karin