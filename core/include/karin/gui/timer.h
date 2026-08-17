#ifndef CORE_KARIN_GUI_TIMER_H
#define CORE_KARIN_GUI_TIMER_H

#include <cstdint>

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "window.h"

namespace karin::gui
{
class Timer
{
public:
    Timer(Window *target, uint32_t intervalMs, const std::function<void(uint32_t)>& handler);
    ~Timer();

    void start();
    void startLater(uint32_t delayMs);
    void stop();

private:
    bool m_isStarted = false;
    bool m_isRegistered = false;

    uint32_t m_intervalMs;
    uint32_t m_count = 0;
    std::function<void(uint32_t)> m_handler;
    uint32_t m_eventHandlerId = 0;

    std::jthread m_timerThread;
    std::mutex m_mtx;
    std::condition_variable_any m_cv;

    Window *m_window;
};
} // karin

#endif //CORE_KARIN_GUI_TIMER_H
