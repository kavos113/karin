#include "x11_application_impl.h"

#include <iostream>
#include <stdexcept>

#include <unistd.h>
#include <sys/eventfd.h>

#include <karin/system/window.h>

#include "x11_window_impl.h"
#include "x11_context.h"

namespace karin
{
X11ApplicationImpl::X11ApplicationImpl()
{
    XSetErrorHandler(errorHandler);
    XSynchronize(X11Context::instance().display(), True);

    int x11fd = ConnectionNumber(X11Context::instance().display());
    int efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (efd == -1)
    {
        throw std::runtime_error("failed to create eventfd");
    }

    m_pollfds[0].fd = x11fd;
    m_pollfds[0].events = POLLIN;
    m_pollfds[1].fd = efd;
    m_pollfds[1].events = POLLIN;
}

X11ApplicationImpl::~X11ApplicationImpl()
{
    if (m_pollfds[1].fd != -1)
    {
        close(m_pollfds[1].fd);
    }
}

void X11ApplicationImpl::addWindow(XlibWindow window, X11WindowImpl* impl)
{
    m_windows[window] = impl;
}

bool X11ApplicationImpl::waitEvent(EventPayload& event)
{
    if (!m_running)
        return false;

    // to return non-empty event
    while (m_running)
    {
        if (!m_eventQueue.empty())
        {
            event = m_eventQueue.front();
            m_eventQueue.pop();
            return true;
        }

        int timeout = (XPending(X11Context::instance().display()) > 0) ? 0 : -1;

        int ret = poll(m_pollfds, 2, timeout);
        if (ret < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            return false;
        }

        // eventfd
        if (m_pollfds[1].revents & (POLLERR|POLLHUP|POLLNVAL))
        {
            return false;
        }
        if (m_pollfds[1].revents & POLLIN)
        {
            uint64_t val;
            read(m_pollfds[1].fd, &val, sizeof(uint64_t));

            m_dispatcher->handlePostActionEvent();
        }

        // x11 fd
        if ((m_pollfds[0].revents & POLLIN) || XPending(X11Context::instance().display()) > 0)
        {
            while (XPending(X11Context::instance().display()) > 0)
            {
                XEvent xevent;
                XNextEvent(X11Context::instance().display(), &xevent);

                if (m_windows.contains(xevent.xany.window))
                {
                    m_windows[xevent.xany.window]->handleEvent(xevent);
                }
            }
        }
    }

    return false;
}

void X11ApplicationImpl::pushEvent(const Event& event, WindowID window)
{
    EventPayload payload = {};
    payload.event = event;
    payload.windowId = window;
    m_eventQueue.push(payload);
}

void X11ApplicationImpl::shutdown()
{
    m_running = false;
}

int X11ApplicationImpl::errorHandler(Display* display, XErrorEvent* error)
{
    char errorText[256];
    XGetErrorText(display, error->error_code, errorText, sizeof(errorText));

    std::cerr << "X11 Error: " << errorText << " (request code: " << error->request_code
        << ", minor code: " << error->minor_code << ")" << std::endl;

    return 0;
}

void X11ApplicationImpl::setDispatcher(ActionEventDispatcher* dispatcher)
{
    m_dispatcher = dispatcher;
}

void X11ApplicationImpl::notifyActionEvent()
{
    uint64_t val = 1;
    ssize_t ret = write(m_pollfds[1].fd, &val, sizeof(uint64_t));
    if (ret == -1)
    {
        throw std::runtime_error("failed to write to eventfd");
    }
}

void X11ApplicationImpl::addActionEvent(const ActionEvent& event)
{
    pushEvent(event, WINDOW_ID_NONE);
}

void X11ApplicationImpl::addTaskEvent(const TaskEvent& event)
{
    pushEvent(event, WINDOW_ID_NONE);
}

} // karin