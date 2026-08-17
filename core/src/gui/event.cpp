#include <karin/gui/event.h>

#include <thread>

#include <karin/gui/window.h>

namespace karin::gui
{
void setTimeout(Window* target, uint32_t milliseconds, const std::function<void()>& handler)
{
    uint32_t id = target->addActionEventHandler([&handler](const std::any& data)
    {
        handler();
    });

    std::thread([milliseconds, target, id]
    {
        std::this_thread::sleep_for(std::chrono::duration(std::chrono::milliseconds(milliseconds)));
        target->triggerActionEvent(id, std::any{});
    }).detach();
}
}
