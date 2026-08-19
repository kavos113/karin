#ifndef KARIN_GUI_APPLICATION_H
#define KARIN_GUI_APPLICATION_H

#include <memory>
#include <vector>
#include <functional>

#include <karin/system/application.h>

#include "window.h"

namespace karin::gui
{
class ApplicationContext;
class ApplicationEventDispatcher;

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
    void sendTaskEvent(const std::function<void()>& task) const;

    void registerDisposable(const std::function<void()>& disposable);

private:
    std::unique_ptr<ApplicationContext> m_context;
    std::unique_ptr<ApplicationEventDispatcher> m_dispatcher;

    std::vector<std::shared_ptr<Window>> m_windows;

    std::vector<std::function<void()>> m_disposables;
};
} // karin::gui

#endif //KARIN_GUI_APPLICATION_H
