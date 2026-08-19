#ifndef CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H
#define CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H

#include <windows.h>

namespace karin
{
class IWinMessageHandler
{
public:
    virtual ~IWinMessageHandler();

    virtual LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam);
};
} // karin

#endif //CORE_SRC_SYSTEM_WINDOWS_WIN_MESSAGE_HANDLER_H
