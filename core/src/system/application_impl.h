#ifndef SYSTEM_APPLICATION_IMPL_H
#define SYSTEM_APPLICATION_IMPL_H

#include <karin/system/event.h>

namespace karin
{
class IApplicationImpl
{
public:
    virtual ~IApplicationImpl() = default;

    virtual void shutdown() = 0;

    virtual bool waitEvent(EventPayload& event) = 0;
};
} // karin

#endif //SYSTEM_APPLICATION_IMPL_H
