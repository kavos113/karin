#ifndef SRC_GUI_CONTAINER_NODE_H
#define SRC_GUI_CONTAINER_NODE_H

#include <memory>
#include <vector>

#include <karin/common/geometry/size.h>
#include <karin/common/geometry/point.h>
#include <karin/graphics/graphics_context.h>
#include "window.h"
#include "view_node.h"

namespace karin::gui
{
class ContainerNode : public ViewNode
{
public:
    enum class LayoutDirection
    {
        Row = 0,
        Column = 1
    };

    enum class WrapMode
    {
        No = 0,
        Wrap = 1,
        WrapReverse = 2
    };

    ContainerNode() = default;
    explicit ContainerNode(Size size) : ViewNode(size) {}
    ~ContainerNode() override = default;

    void onAttachToWindow(Window* window) override;
    void onDetachFromWindow() override;

    void addChild(std::unique_ptr<ViewNode> child);
    void removeChild(ViewNode* child);
    void clearChildren();
    void setLayoutDirection(LayoutDirection direction);
    void setGap(float gap);
    void setWrapMode(WrapMode mode);
    void setEnableClip(bool enable);

    ViewNode* hitTest(const Point& point) override;

protected:
    void drawInternal(GraphicsContext& gc) const final;

    virtual void drawBackground(GraphicsContext& gc) const {}
    virtual void drawForeground(GraphicsContext& gc) const {}

    std::vector<std::unique_ptr<ViewNode>> m_children;

    bool m_enableClip = false;
};
} // karin::gui

#endif //SRC_GUI_CONTAINER_NODE_H