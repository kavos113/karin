#include <karin/gui/timer.h>

#include <chrono>

namespace karin::gui
{
Timer::Timer(Window *target, uint32_t intervalMs, const std::function<void(uint32_t)>& handler)
    : m_window(target), m_intervalMs(intervalMs), m_handler(handler)
{
}

Timer::~Timer()
{
    if (m_isStarted)
    {
        stop();
    }
    if (m_isRegistered)
    {
        m_window->clearActionEvent(m_eventHandlerId);
    }
}

void Timer::start()
{
    if (m_isStarted)
    {
        return;
    }

    if (!m_handler)
    {
        return;
    }

    if (!m_isRegistered)
    {
        m_eventHandlerId = m_window->addActionEventHandler([this](const std::any& data)
        {
            m_handler(std::any_cast<uint32_t>(m_count));
        });
    }

    m_timerThread = std::jthread([this](const std::stop_token& s)
    {
        while (!s.stop_requested())
        {
            m_window->triggerActionEvent(m_eventHandlerId, std::any(m_count));

            std::unique_lock lock(m_mtx);
            m_cv.wait_for(lock, s, std::chrono::milliseconds(m_intervalMs), []{ return false; });

            if (s.stop_requested())
            {
                return;
            }
        }
    });

    m_isStarted = true;
    m_isRegistered = true;
}

void Timer::startLater(uint32_t delayMs)
{
    if (m_isStarted)
    {
        return;
    }

    if (!m_handler)
    {
        return;
    }

    if (!m_isRegistered)
    {
        m_eventHandlerId = m_window->addActionEventHandler([this](const std::any& data)
        {
            m_handler(std::any_cast<uint32_t>(m_count));
        });
    }

    m_timerThread = std::jthread([this, delayMs](const std::stop_token& s)
    {
        std::unique_lock lock(m_mtx);
        m_cv.wait_for(lock, s, std::chrono::milliseconds(delayMs), []{ return false; });

        if (s.stop_requested())
        {
            return;
        }

        while (!s.stop_requested())
        {
            m_window->triggerActionEvent(m_eventHandlerId, std::any(m_count));

            std::unique_lock l(m_mtx);
            m_cv.wait_for(l, s, std::chrono::milliseconds(m_intervalMs), []{ return false; });

            if (s.stop_requested())
            {
                return;
            }
        }
    });

    m_isStarted = true;
    m_isRegistered = true;
}

void Timer::stop()
{
    if (!m_isStarted || !m_isRegistered)
    {
        return;
    }

    m_timerThread.request_stop();
    m_isStarted = false;
}
} // karin