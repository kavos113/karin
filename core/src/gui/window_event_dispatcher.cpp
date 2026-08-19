#include "window_event_dispatcher.h"

#include <karin/common/geometry/point.h>

namespace karin::gui
{
WindowEventDispatcher::WindowEventDispatcher(ViewNode* rootView)
    : m_rootView(rootView)
{
}

void WindowEventDispatcher::dispatchEvent(const Event& event)
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
            else if constexpr (std::is_same_v<T, KeyEvent>)
            {
                handleKeyEvent(e);
            }
            else if constexpr (std::is_same_v<T, KeyTypeEvent>)
            {
                handleKeyTypeEvent(e);
            }
        },
        event
    );
}

void WindowEventDispatcher::handleMouseMoveEvent(const MouseMoveEvent& event)
{
    Point point(static_cast<float>(event.x), static_cast<float>(event.y));

    ViewNode* target = m_rootView->hitTest(point, ViewNode::EventType::PointerMove);
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

void WindowEventDispatcher::handleMouseButtonEvent(const MouseButtonEvent& event)
{
    Point point(static_cast<float>(event.x), static_cast<float>(event.y));

    ViewNode* target = m_rootView->hitTest(point, ViewNode::EventType::PointerClick);
    if (target)
    {
        if (event.type == MouseButtonEvent::Type::ButtonPress_)
        {
            if (target->isFocusable())
            {
                m_focusNode = target;
                target->triggerChangeFocusStateHandler(true);
            }
            else
            {
                if (m_focusNode)
                {
                    m_focusNode->triggerChangeFocusStateHandler(false);
                }
                m_focusNode = nullptr;
            }
        }

        switch (event.type)
        {
        case MouseButtonEvent::Type::ButtonPress_:
            target->triggerPointerDownHandler(point, event.button);
            break;

        case MouseButtonEvent::Type::ButtonRelease_:
            target->triggerPointerUpHandler(point, event.button);
            break;
        }
    }
    else
    {
        if (m_focusNode)
        {
            m_focusNode->triggerChangeFocusStateHandler(false);
        }
        m_focusNode = nullptr;
    }
}

void WindowEventDispatcher::handleMouseWheelEvent(const MouseWheelEvent& event) const
{
    Point point(static_cast<float>(event.x), static_cast<float>(event.y));

    ViewNode* target = m_rootView->hitTest(point, ViewNode::EventType::MouseWheel);
    if (target)
    {
        target->triggerMouseWheelHandler(point, event.delta);
    }
}

void WindowEventDispatcher::handleKeyEvent(const KeyEvent& event) const
{
    if (m_focusNode)
    {
        m_focusNode->triggerKeyHandler(event);
    }
    else
    {
        m_rootView->triggerKeyHandler(event);
    }
}

void WindowEventDispatcher::handleKeyTypeEvent(const KeyTypeEvent& event) const
{
    if (m_focusNode)
    {
        m_focusNode->triggerKeyTypeHandler(event.character);
    }
    else
    {
        m_rootView->triggerKeyTypeHandler(event.character);
    }
}
} // karin::gui
