#include <karin/gui/container_node.h>

#include <algorithm>
#include <ranges>

#include <yoga/Yoga.h>

#include <karin/common/geometry/rectangle.h>

namespace
{
YGFlexDirection toYogaFlexDirection(karin::gui::ContainerNode::LayoutDirection direction)
{
    switch (direction)
    {
    case karin::gui::ContainerNode::LayoutDirection::Row:
        return YGFlexDirectionRow;
    case karin::gui::ContainerNode::LayoutDirection::Column:
        return YGFlexDirectionColumn;
    default:
        return YGFlexDirectionRow; // Default to Row if unknown
    }
}

YGWrap toYogaWrap(karin::gui::ContainerNode::WrapMode mode)
{
    switch (mode)
    {
    case karin::gui::ContainerNode::WrapMode::No:
        return YGWrapNoWrap;
    case karin::gui::ContainerNode::WrapMode::Wrap:
        return YGWrapWrap;
    case karin::gui::ContainerNode::WrapMode::WrapReverse:
        return YGWrapWrapReverse;
    default:
        return YGWrapNoWrap; // Default to No Wrap if unknown
    }
}
}

namespace karin::gui
{
void ContainerNode::onAttachToWindow(Window* window)
{
    ViewNode::onAttachToWindow(window);

    for (const auto& child : m_children)
    {
        child->onAttachToWindow(window);
    }
}

void ContainerNode::onDetachFromWindow()
{
    ViewNode::onDetachFromWindow();

    for (const auto& child : m_children)
    {
        child->onDetachFromWindow();
    }
}

void ContainerNode::drawInternal(GraphicsContext& gc) const
{
    Rectangle layout = getLayout();
    drawBackground(gc);

    gc.withSave([&gc, layout, this]
    {
        gc.multiplyTransform(Transform2D().translate(layout.pos.x, layout.pos.y));

        if (m_enableClip)
        {
            Rectangle clipRect(0, 0, layout.size.width, layout.size.height);
            gc.clip(clipRect);
        }

        for (const auto& child : m_children)
        {
            child->draw(gc);
        }
    });

    drawForeground(gc);
}

bool ContainerNode::needLayer() const
{
    return ViewNode::needLayer() && !m_children.empty();
}

void ContainerNode::addChild(ViewNode* child)
{
    if (m_window)
    {
        child->onAttachToWindow(m_window);
    }

    YGNodeInsertChild(m_yogaNode, child->getYogaNode(), YGNodeGetChildCount(m_yogaNode));

    m_children.push_back(child);
}

void ContainerNode::insertChild(ViewNode* child, int index)
{
    if (m_window)
    {
        child->onAttachToWindow(m_window);
    }

    if (index < 0 || index > m_children.size())
    {
        return;
    }

    YGNodeInsertChild(m_yogaNode, child->getYogaNode(), index);

    m_children.insert(m_children.begin() + index, child);
}

void ContainerNode::removeChild(ViewNode* child)
{
    auto it = std::ranges::find_if(m_children,
        [child](const ViewNode* ptr) { return ptr == child; });

    if (it != m_children.end())
    {
        if (m_window)
        {
            (*it)->onDetachFromWindow();
        }

        YGNodeRemoveChild(m_yogaNode, (*it)->getYogaNode());
        m_children.erase(it);
    }
}

void ContainerNode::clearChildren()
{
    for (const auto& child : m_children)
    {
        if (m_window)
        {
            child->onDetachFromWindow();
        }
        YGNodeRemoveChild(m_yogaNode, child->getYogaNode());
    }
    m_children.clear();
}

void ContainerNode::setLayoutDirection(LayoutDirection direction)
{
    YGNodeStyleSetFlexDirection(m_yogaNode,  toYogaFlexDirection(direction));
}

void ContainerNode::setGap(float gap)
{
    YGNodeStyleSetGap(m_yogaNode, YGGutterAll, gap);
}

void ContainerNode::setWrapMode(WrapMode mode)
{
    YGNodeStyleSetFlexWrap(m_yogaNode, toYogaWrap(mode));
}

void ContainerNode::setEnableClip(bool enable)
{
    m_enableClip = enable;
}

ViewNode* ContainerNode::hitTest(const Point& point, EventType type)
{
    float width = YGNodeLayoutGetWidth(m_yogaNode);
    float height = YGNodeLayoutGetHeight(m_yogaNode);

    if (point.x < 0 || point.y < 0 || point.x > width || point.y > height)
    {
        return nullptr;
    }

    for (const auto & child : m_children)
    {
        float childX = YGNodeLayoutGetLeft(child->getYogaNode());
        float childY = YGNodeLayoutGetTop(child->getYogaNode());
        Point childPoint = { point.x - childX, point.y - childY };

        ViewNode* hitNode = child->hitTest(childPoint, type);
        if (hitNode)
        {
            return hitNode;
        }
    }

    if (!m_enableHandlers[static_cast<size_t>(type)])
    {
        return nullptr;
    }

    return this;
}
} // karin::gui