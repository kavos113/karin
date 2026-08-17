#ifndef CORE_KARIN_GUI_EVENT_H
#define CORE_KARIN_GUI_EVENT_H

#include <cstdint>

#include <functional>

#include "window.h"

namespace karin::gui
{
void setTimeout(Window *target, uint32_t milliseconds, const std::function<void()>& handler);
void setTimer(Window *target, uint32_t intervalMs, const std::function<void()>& handler);
void setCountTimer(Window *target, uint32_t intervalMs, const std::function<void(uint32_t)>& handler);
}

#endif //CORE_KARIN_GUI_EVENT_H
