#ifndef CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H
#define CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H

#include <windows.h>

namespace karin
{
class IWinMessageHandler
{
public:
    virtual ~IWinMessageHandler() = default;

    virtual LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;
    virtual void setHwnd(HWND hwnd) = 0;
};

inline LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    IWinMessageHandler * self = nullptr;

    if (message == WM_CREATE)
    {
        auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = static_cast<IWinMessageHandler*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));

        self->setHwnd(hwnd);
    }
    else
    {
        self = reinterpret_cast<IWinMessageHandler*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (self)
    {
        return self->handleMessage(message, wParam, lParam);
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

} // karin

#endif //CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H
