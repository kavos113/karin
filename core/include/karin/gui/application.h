#ifndef KARIN_GUI_APPLICATION_H
#define KARIN_GUI_APPLICATION_H

#include "window.h"

#include <karin/system/application.h>

#include <memory>
#include <vector>

#include "application_event_dispatcher.h"

namespace karin::gui
{
class ApplicationContext;

class Application
{
public:
    Application();
    ~Application();

    std::shared_ptr<Window> createWindow(
        const std::string& title,
        int x,
        int y,
        int width,
        int height
    );

    void run();

    uint32_t addActionEventHandler(const std::function<void(std::any)>& handler) const;
    void clearActionEvent(uint32_t id) const;
    void sendActionEvent(uint32_t id, const std::any& data) const;

private:
    std::unique_ptr<ApplicationContext> m_context;
    std::unique_ptr<ApplicationEventDispatcher> m_dispatcher;

    std::vector<std::shared_ptr<Window>> m_windows;
};
} // karin::gui

#endif //KARIN_GUI_APPLICATION_H
