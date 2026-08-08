#include "event_dispatcher.h"

#include <karin/common/geometry/point.h>

namespace karin::gui
{
EventDispatcher::EventDispatcher(ViewNode* rootView)
    : m_rootView(rootView)
{
}

void EventDispatcher::dispatchEvent(const Event& event)
{
    if (!m_rootView)
    {
        return;
    }

    std::visit(
        [this]<typename T0>(const T0& e)
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, MouseMoveEvent>)
            {
                handleMouseMoveEvent(e);
            }
            else if constexpr (std::is_same_v<T, MouseButtonEvent>)
            {
                handleMouseButtonEvent(e);
            }
            else if constexpr (std::is_same_v<T, MouseWheelEvent>)
            {
                handleMouseWheelEvent(e);
            }
        },
        event
    );
}

void EventDispatcher::handleMouseMoveEvent(const MouseMoveEvent& event)
{
    using enum ViewNode::EventType;

    Point point(static_cast<float>(event.x), static_cast<float>(event.y));

    ViewNode* target = m_rootView->hitTest(point, PointerMove);
    if (target)
    {
        target->triggerPointerMoveHandler(point);
    }

    if (target && m_hoveredNode != target)
    {
        m_hoveredNode = target;
        target->triggerPointerEnterHandler(point);
    }

    if (m_hoveredNode && m_hoveredNode != target)
    {
        m_hoveredNode->triggerPointerLeaveHandler(point);
        m_hoveredNode = target;
    }
}

void EventDispatcher::handleMouseButtonEvent(const MouseButtonEvent& event)
{
    using enum ViewNode::EventType;

    Point point(static_cast<float>(event.x), static_cast<float>(event.y));
    auto type = PointerUp;

    switch (event.type)
    {
    case MouseButtonEvent::Type::ButtonPress_:
        type = PointerDown;
        break;

    case MouseButtonEvent::Type::ButtonRelease_:
        type = PointerUp;
        break;
    }

    ViewNode* target = m_rootView->hitTest(point, type);

    if (target)
    {
        if (target->isFocusable())
        {
            m_focusNode = target;
        }
        else
        {
            m_focusNode = nullptr;
        }

        if (type == PointerDown)
        {
            target->triggerPointerDownHandler(point, event.button);
        }
        else
        {
            target->triggerPointerUpHandler(point, event.button);
        }
    }
    else
    {
        m_focusNode = nullptr;
    }
}

void EventDispatcher::handleMouseWheelEvent(const MouseWheelEvent& event) const
{
    using enum ViewNode::EventType;

    Point point(static_cast<float>(event.x), static_cast<float>(event.y));

    ViewNode* target = m_rootView->hitTest(point, MouseWheel);
    if (target)
    {
        target->triggerMouseWheelHandler(point, event.delta);
    }
}
} // karin::gui
