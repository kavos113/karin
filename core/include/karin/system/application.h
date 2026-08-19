#ifndef KARIN_SYSTEM_APPLICATION_H
#define KARIN_SYSTEM_APPLICATION_H

#include <cstdint>

#include <functional>
#include <memory>
#include <unordered_map>

#include "window.h"
#include "event.h"

namespace karin
{
class IApplicationImpl;
class ActionEventDispatcher;

class Application
{
public:
    static Application& instance();

    // return true if event loop should continue, false to quit.
    bool waitEvent(EventPayload& event) const;

    // TODO: peekEvent

    std::unique_ptr<Window> createWindow(
        const std::string& title,
        int x = 0,
        int y = 0,
        int width = Window::DEFAULT_WIDTH,
        int height = Window::DEFAULT_HEIGHT
    ) const;

    std::unique_ptr<Window> createWindow(
        const std::string& title,
        Rectangle rect = Rectangle(0, 0, Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT)
    ) const;

    WindowID registerWindow(Window* window);
    void unregisterWindow(WindowID id);
    Window* findWindow(WindowID id) const;

    void sendAction(uint32_t actionId, const std::any& data) const;
    void sendTask(const std::function<void()>& task) const;

private:
    Application();
    ~Application() = default;

    std::unique_ptr<IApplicationImpl> m_impl;
    std::unique_ptr<ActionEventDispatcher> m_eventDispatcher;

    std::unordered_map<WindowID, Window*> m_windowRegistry;
    WindowID m_nextWindowID = 1;
};
} // karin

#endif //KARIN_SYSTEM_APPLICATION_H
