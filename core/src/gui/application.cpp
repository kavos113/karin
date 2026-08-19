#include <karin/gui/application.h>
#include "application_context.h"
#include "application_event_dispatcher.h"

namespace karin::gui
{
static ApplicationContext *s_appContext = nullptr;

Application::Application()
{
    if (s_appContext)
    {
        throw std::runtime_error("Application already exists");
    }

    m_context = std::make_unique<ApplicationContext>();
    m_dispatcher = std::make_unique<ApplicationEventDispatcher>();
    s_appContext = m_context.get();
}

Application::~Application()
{
    for (const auto & disposable : m_disposables)
    {
        try
        {
            disposable();
        }
        catch (...)
        {
            std::cerr << "exception in disposable" << std::endl;
            continue;
        }
    }

    s_appContext = nullptr;
}

std::shared_ptr<Window> Application::createWindow(const std::string& title, int x, int y, int width, int height)
{
    auto window = std::make_shared<Window>(title, x, y, width, height);
    m_windows.push_back(window);

    return window;
}

void Application::run()
{
    for (auto& window : m_windows)
    {
        window->beforeRun();
    }

    karin::Application& app = karin::Application::instance();
    EventPayload event;
    while (app.waitEvent(event))
    {
        if (std::holds_alternative<std::monostate>(event.event))
        {
            continue;
        }

        if (isApplicationEvent(event.event))
        {
            m_dispatcher->dispatchEvent(event.event);
        }
        else
        {
            karin::Window* window = app.findWindow(event.windowId);
            if (!window)
            {
                continue;
            }

            void* userData = window->userData();
            if (userData)
            {
                auto* guiWindow = static_cast<Window*>(userData);
                guiWindow->dispatchEvent(event.event);
            }
        }
    }
}

uint32_t Application::addActionEventHandler(const std::function<void(std::any)>& handler) const
{
    return m_dispatcher->addActionEventHandler(handler);
}

void Application::clearActionEvent(uint32_t id) const
{
    m_dispatcher->clearActionEvent(id);
}

void Application::sendActionEvent(uint32_t id, const std::any& data) const
{
    karin::Application& app = karin::Application::instance();
    app.sendAction(id, data);
}

void Application::sendTaskEvent(const std::function<void()>& task) const
{
    karin::Application& app = karin::Application::instance();
    app.sendTask(task);
}

void Application::registerDisposable(const std::function<void()>& disposable)
{
    m_disposables.push_back(disposable);
}

ApplicationContext& getAppContext()
{
    if (!s_appContext)
    {
        throw std::runtime_error("Application context is not initialized");
    }
    return *s_appContext;
}
} // karin::gui