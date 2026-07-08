#ifndef COMMON_X11_WINDOW_H
#define COMMON_X11_WINDOW_H

#include <X11/Xlib.h>

// conflict between Xlib Window and Window from system/window.h
using XlibWindow = Window;

#endif //COMMON_X11_WINDOW_H
