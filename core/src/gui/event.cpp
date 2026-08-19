#include <karin/gui/event.h>

#include <thread>
#include <mutex>
#include <condition_variable>

namespace karin::gui
{
void setTimeout(Application* target, uint32_t milliseconds, const std::function<void()>& handler)
{
    uint32_t id = target->addActionEventHandler([handler](const std::any& data)
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

    state->th = std::jthread([milliseconds, target, id, state](const std::stop_token& s)
    {
        std::unique_lock lock(state->mtx);
        state->cv.wait_for(lock, s, std::chrono::milliseconds(milliseconds), []{ return false; });

        if (s.stop_requested())
        {
            return;
        }

        target->sendActionEvent(id, std::any());
    });

    target->registerDisposable([state]
    {
        state->th.request_stop();
    });
}
}
