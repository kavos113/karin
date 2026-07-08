#ifndef SYSTEM_WINDOWS_WIN_WINDOW_CLASS_REGISTRY_H
#define SYSTEM_WINDOWS_WIN_WINDOW_CLASS_REGISTRY_H

#include <stdexcept>
#include <vector>
#include <windows.h>

namespace karin
{
class WinWindowClassRegistry
{
public:
    void registerClass(WNDCLASSEX& wc, const std::wstring& className);
    void unregisterClasses() const;

private:
    std::vector<std::wstring> m_registeredClasses;
};
} // karin

#endif //SYSTEM_WINDOWS_WIN_WINDOW_CLASS_REGISTRY_H
