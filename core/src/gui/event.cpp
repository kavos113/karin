#include <karin/gui/event.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <karin/gui/application.h>
#include "application_context.h"

namespace karin::gui
{
void setTimeout(uint32_t milliseconds, const std::function<void()>& handler)
{
    uint32_t id = getAppContext().addActionEventHandler([handler](const std::any& data)
    {
        handler();
    });

    struct TimeoutState
    {
        std::mutex mtx;
        std::condition_variable_any cv;
        std::jthread th;
    };
    auto state = std::make_shared<TimeoutState>();

    state->th = std::jthread([milliseconds, id, state](const std::stop_token& s)
    {
        std::unique_lock lock(state->mtx);
        state->cv.wait_for(lock, s, std::chrono::milliseconds(milliseconds), []{ return false; });

        if (s.stop_requested())
        {
            return;
        }

        getAppContext().sendActionEvent(id, std::any());
    });

    getAppContext().registerDisposable([state]
    {
        state->th.request_stop();
    });
}
}
