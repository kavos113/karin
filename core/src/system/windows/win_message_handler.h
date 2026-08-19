#ifndef CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H
#define CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H

#include <windows.h>

namespace karin
{
class IWinMessageHandler
{
public:
    virtual ~IWinMessageHandler() = 0;

    virtual LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;
    virtual void setHwnd(HWND hwnd) = 0;
};

inline LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
} // karin

#endif //CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H
