#ifndef CORE_KARIN_GUI_EVENT_H
#define CORE_KARIN_GUI_EVENT_H

#include <cstdint>

#include <functional>

namespace karin::gui
{
void setTimeout(uint32_t milliseconds, const std::function<void()>& handler);
}

#endif //CORE_KARIN_GUI_EVENT_H
