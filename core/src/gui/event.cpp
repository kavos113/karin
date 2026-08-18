#include <karin/gui/event.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <karin/gui/window.h>

namespace karin::gui
{
void setTimeout(Window* target, uint32_t milliseconds, const std::function<void()>& handler)
{
    uint32_t id = target->addActionEventHandler([handler](const std::any& data)
    {
        handler();
    });

    std::mutex mtx;
    std::condition_variable_any cv;

    std::jthread th([milliseconds, target, id, &mtx, &cv](const std::stop_token& s)
    {
        std::unique_lock lock(mtx);
        cv.wait_for(lock, s, std::chrono::milliseconds(milliseconds), []{ return false; });

        if (s.stop_requested())
        {
            return;
        }

        target->triggerActionEvent(id, std::any{});
    });

    target->registerDisposable([&th]
    {
        th.request_stop();
    });
}
}
