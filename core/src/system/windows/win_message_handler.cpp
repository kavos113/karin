#include "win_message_handler.h"

namespace karin
{
LRESULT windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
}
