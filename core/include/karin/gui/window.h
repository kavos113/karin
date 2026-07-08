#ifndef KARIN_GUI_WINDOW_H
#define KARIN_GUI_WINDOW_H

#include <karin/system/window.h>
#include <karin/system/event.h>
#include <karin/graphics/window_renderer.h>

#include "view_node.h"

#include <memory>
#include <string>

namespace karin::gui
{
class Application;
class EventDispatcher;

class Window
{
private:
    friend class Application;

    void beforeRun();

public:
    Window(
        const std::string& title,
        int x,
        int y,
        int width,
        int height
    );
    ~Window();

    void setRootView(std::unique_ptr<ViewNode> rootView);
    void requestRelayout();

    void dispatchEvent(const Event& event) const;

private:

    std::unique_ptr<ViewNode> m_rootView;

    std::unique_ptr<karin::Window> m_window;
    std::unique_ptr<WindowRenderer> m_renderer;
    std::unique_ptr<EventDispatcher> m_eventDispatcher;

    bool m_needRelayout = true;
};
} // karin::gui

#endif //KARIN_GUI_WINDOW_H
