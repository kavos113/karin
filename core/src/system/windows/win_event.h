#ifndef SYSTEM_WINDOWS_WIN_EVENT_H
#define SYSTEM_WINDOWS_WIN_EVENT_H

#include <windows.h>
#include <karin/system/event.h>

#include <optional>

namespace karin
{
std::optional<Event> translateWinEvent(UINT message, WPARAM wParam, LPARAM lParam);
} // karin

#endif //SYSTEM_WINDOWS_WIN_EVENT_H
