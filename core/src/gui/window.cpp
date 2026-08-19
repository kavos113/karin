#include <karin/gui/window.h>

#include <karin/system/application.h>
#include <karin/common/color/color.h>

#include "window_event_dispatcher.h"

namespace karin::gui
{
Window::Window(const std::string& title, int x, int y, int width, int height)
{
    karin::Application& app = karin::Application::instance();
    m_window = app.createWindow(title, x, y, width, height);
    m_window->setUserData(this);

    m_renderer = std::make_unique<WindowRenderer>(m_window.get());
    m_renderer->setClearColor(Color(Color::White));
}

Window::~Window()
{
    for (const auto & disposable : m_disposables)
    {
        disposable();
    }

    m_renderer->cleanUp();
}

void Window::beforeRun()
{
    m_rootView->calculateLayout();
    m_needRelayout = false;

    m_renderer->addDrawCommand(
        [this](GraphicsContext& gc)
        {
            if (m_needRelayout)
            {
                m_rootView->calculateLayout();
                m_needRelayout = false;
            }

            m_rootView->draw(gc);
        }
    );

    m_window->addResizeCallback(
        [this](Size size)
        {
            if (size.width == 0 || size.height == 0)
            {
                return;
            }
            requestRelayout();
            m_rootView->setSize(size);
        }
    );

    m_renderer->update();
    m_window->setStatus(karin::Window::ShowStatus::SHOW);
}

void Window::dispatchEvent(const Event& event) const
{
    if (m_eventDispatcher)
    {
        m_eventDispatcher->dispatchEvent(event);
    }
}

uint32_t Window::addActionEventHandler(const std::function<void(std::any)>& handler) const
{
    if (m_eventDispatcher)
    {
        return m_eventDispatcher->addActionEventHandler(handler);
    }

    return 0;
}

void Window::triggerActionEvent(uint32_t id, const std::any& data) const
{
    m_window->sendActionEvent(id, data);
}

void Window::clearActionEvent(uint32_t id) const
{
    if (m_eventDispatcher)
    {
        m_eventDispatcher->clearActionEvent(id);
    }
}

void Window::registerDisposable(const std::function<void()>& disposable)
{
    m_disposables.push_back(disposable);
}

void Window::setRootView(std::unique_ptr<ViewNode> rootView)
{
    m_rootView = std::move(rootView);
    m_eventDispatcher = std::make_unique<WindowEventDispatcher>(m_rootView.get());

    m_rootView->onAttachToWindow(this);
}

void Window::requestRelayout()
{
    m_needRelayout = true;
    m_window->invalidate();
}

void Window::requestRedraw() const
{
    m_window->invalidate();
}
} // karin::gui