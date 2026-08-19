#include <karin/system/application.h>

#include "application_impl.h"
#include "action_event_dispatcher.h"
#include "platform.h"

namespace karin
{
Application& Application::instance()
{
    static Application instance;

    return instance;
}

bool Application::waitEvent(EventPayload &event) const
{
    return m_impl->waitEvent(event);
}

std::unique_ptr<Window> Application::createWindow(const std::string& title, int x, int y, int width, int height) const
{
    return std::make_unique<Window>(
        m_impl.get(),
        title,
        x,
        y,
        width,
        height
    );
}

std::unique_ptr<Window> Application::createWindow(const std::string& title, Rectangle rect) const
{
    return std::make_unique<Window>(
        m_impl.get(),
        title,
        rect
    );
}

WindowID Application::registerWindow(Window* window)
{
    WindowID id = m_nextWindowID++;
    m_windowRegistry[id] = window;
    return id;
}

void Application::unregisterWindow(WindowID id)
{
    m_windowRegistry.erase(id);
}

Window* Application::findWindow(WindowID id) const
{
    if (auto it = m_windowRegistry.find(id); it != m_windowRegistry.end())
    {
        return it->second;
    }
    return nullptr;
}

Application::Application()
{
    auto [impl, manager] = createApplicationImpl();

    m_impl = std::move(impl);
    m_eventDispatcher = std::make_unique<ActionEventDispatcher>(manager);
    manager->setDispatcher(m_eventDispatcher.get());
}

void Application::sendAction(uint32_t actionId, const std::any& data) const
{
    m_eventDispatcher->sendAction(actionId, data);
}

void Application::sendTask(const std::function<void()>& task) const
{
    m_eventDispatcher->sendTask(task);
}
}
