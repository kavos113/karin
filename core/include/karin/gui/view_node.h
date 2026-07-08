#ifndef KARIN_GUI_VIEW_NODE_H
#define KARIN_GUI_VIEW_NODE_H

#include <karin/common/geometry/size.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/graphics_context.h>

#include <yoga/Yoga.h>
#include <array>
#include <functional>
#include <optional>

namespace karin::gui
{
struct NodeBorder
{
    enum class LineStyle
    {
        None,
        Solid,
        Dashed,
        Dotted
    };

    float width;
    Color color;
    LineStyle style;
};

class Window;

class ViewNode
{
public:
    enum class Side : uint8_t
    {
        Left = 0,
        Top = 1,
        Right = 2,
        Bottom = 3,
        Horizontal = 4,
        Vertical = 5,
        All = 6
    };

    ViewNode();
    explicit ViewNode(Size size);
    virtual ~ViewNode();

    void draw(GraphicsContext& gc) const;
    virtual ViewNode* hitTest(const Point& point);

    void calculateLayout() const;
    Rectangle getLayout() const;

    virtual void onAttachToWindow(Window* window);
    virtual void onDetachFromWindow();
    void requestRelayout();

    void setSize(Size size);
    void setWidth(float width);
    void setHeight(float height);
    void setMargin(Side side, float margin);
    void setPadding(Side side, float padding);
    void setBorder(Side side, float width, Color color, NodeBorder::LineStyle style);
    void setBackgroundColor(Color color);

    YGNodeRef getYogaNode() const;

    void setOnClick(std::function<void(Point point)> onClick);
    void triggerClick(Point point) const;

protected:
    virtual void drawInternal(GraphicsContext& gc) const = 0;

    YGNodeRef m_yogaNode;
    Window *m_window = nullptr;

private:
    void drawBorder(GraphicsContext& gc) const;
    void drawBackgroundColor(GraphicsContext& gc) const;

    std::array<NodeBorder, 4> m_borders;
    std::optional<Color> m_backgroundColor = std::nullopt;
    std::function<void(Point point)> m_onClick;
};
} // karin

#endif //KARIN_GUI_VIEW_NODE_H
