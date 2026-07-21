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
    enum class LineStyle : uint8_t
    {
        None = 0,
        Solid = 1,
        Dashed = 2,
        Dotted = 3
    };

    float width;
    Color color;
    LineStyle style;
};

struct ShadowParams
{
    float offsetX;
    float offsetY;
    Color color;
    float blurRadius = 0.0f;
    float spreadRadius = 0.0f;
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
    void requestRelayout() const;
    void requestRedraw() const;

    void setSize(Size size);
    void setWidth(float width);
    void setHeight(float height);
    void setMargin(Side side, float margin);
    void setPadding(Side side, float padding);
    void setBorder(Side side, float width, Color color, NodeBorder::LineStyle style);
    void setBackgroundColor(Color color);
    void setOpacity(float opacity);
    void setShadow(float offsetX, float offsetY, Color color, float blurRadius = 0.0f, float spreadRadius = 0.0f);

    YGNodeRef getYogaNode() const;

    void setOnClick(std::function<void(Point point)> onClick);
    void triggerClick(Point point) const;

protected:
    virtual void drawInternal(GraphicsContext& gc) const = 0;
    virtual bool needLayer() const;

    YGNodeRef m_yogaNode;
    Window *m_window = nullptr;

private:
    void drawBorder(GraphicsContext& gc) const;
    void drawBackgroundColor(GraphicsContext& gc) const;
    void drawShadow(GraphicsContext& gc) const;

    std::array<NodeBorder, 4> m_borders;
    std::optional<Color> m_backgroundColor = std::nullopt;
    std::optional<ShadowParams> m_shadow = std::nullopt;
    std::function<void(Point point)> m_onClick;
    float m_opacity = 1.0f;
};
} // karin

#endif //KARIN_GUI_VIEW_NODE_H
