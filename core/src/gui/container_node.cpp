#include <karin/gui/container_node.h>

#include <algorithm>
#include <ranges>

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
    drawBackground(gc);

    gc.withSave([&]
    {
        Rectangle layout = getLayout();
        gc.multiplyTransform(Transform2D().translate(layout.pos.x, layout.pos.y));
        for (const auto& child : m_children)
        {
            child->draw(gc);
        }
    });

    drawForeground(gc);
}

void ContainerNode::addChild(std::unique_ptr<ViewNode> child)
{
    if (m_window)
    {
        child->onAttachToWindow(m_window);
    }

    YGNodeInsertChild(m_yogaNode, child->getYogaNode(), YGNodeGetChildCount(m_yogaNode));

    m_children.push_back(std::move(child));
}

void ContainerNode::removeChild(ViewNode* child)
{
    auto it = std::ranges::find_if(m_children,
        [child](const std::unique_ptr<ViewNode>& ptr) { return ptr.get() == child; });

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

ViewNode* ContainerNode::hitTest(const Point& point)
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

        ViewNode* hitNode = child->hitTest(childPoint);
        if (hitNode)
        {
            return hitNode;
        }
    }

    return this;
}
} // karin::gui