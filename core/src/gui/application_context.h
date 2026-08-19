#ifndef GUI_APPLICATION_CONTEXT_H
#define GUI_APPLICATION_CONTEXT_H

#include <memory>
#include <functional>
#include <any>

#include <karin/graphics/text_engine.h>
#include <karin/gui/application.h>

#include "application_event_dispatcher.h"

namespace karin::gui
{
class ApplicationContext
{
public:
    ApplicationContext();
    ~ApplicationContext();

    uint32_t addActionEventHandler(const std::function<void(std::any)>& handler) const;
    void clearActionEvent(uint32_t id) const;
    void sendActionEvent(uint32_t id, const std::any& data) const;
    void sendTaskEvent(const std::function<void()>& task) const;

    void registerDisposable(const std::function<void()>& disposable);

    void flushTasks();

    std::unique_ptr<TextEngine> textEngine;

private:
    friend Application;

    std::unique_ptr<ApplicationEventDispatcher> m_dispatcher;
    std::vector<std::function<void()>> m_disposables;
};

ApplicationContext& getAppContext();
} // karin::gui

#endif //GUI_APPLICATION_CONTEXT_H
