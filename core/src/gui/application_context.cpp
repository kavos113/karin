#include "application_context.h"

namespace karin::gui
{
ApplicationContext::ApplicationContext()
{
    textEngine = std::make_unique<TextEngine>();
    m_dispatcher = std::make_unique<ApplicationEventDispatcher>();
}

ApplicationContext::~ApplicationContext()
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
}

uint32_t ApplicationContext::addActionEventHandler(const std::function<void(std::any)>& handler) const
{
    return m_dispatcher->addActionEventHandler(handler);
}

void ApplicationContext::clearActionEvent(uint32_t id) const
{
    m_dispatcher->clearActionEvent(id);
}

void ApplicationContext::sendActionEvent(uint32_t id, const std::any& data) const
{
    karin::Application& app = karin::Application::instance();
    app.sendAction(id, data);
}

void ApplicationContext::sendTaskEvent(const std::function<void()>& task) const
{
    karin::Application& app = karin::Application::instance();
    app.sendTask(task);
}

void ApplicationContext::registerDisposable(const std::function<void()>& disposable)
{
    m_disposables.push_back(disposable);
}
} // karin::gui