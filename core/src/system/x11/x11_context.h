#ifndef SYSTEM_X11_X11_CONTEXT_H
#define SYSTEM_X11_X11_CONTEXT_H

#include <X11/Xlib.h>

namespace karin
{
class X11Context
{
public:
    static X11Context& instance();

    Display *display() const;
private:
    X11Context();
    ~X11Context();

    Display *m_display = nullptr;
};
} // karin

#endif //SYSTEM_X11_X11_CONTEXT_H
