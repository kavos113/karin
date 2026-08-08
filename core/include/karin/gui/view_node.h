#ifndef KARIN_GUI_VIEW_NODE_H
#define KARIN_GUI_VIEW_NODE_H

#include <array>
#include <bitset>
#include <functional>
#include <optional>
#include <vector>

#include <yoga/Yoga.h>

#include <karin/common/geometry/size.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/system/event.h>
#include <karin/graphics/graphics_context.h>

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

    enum class EventType : uint8_t
    {
        PointerMove = 0, // TODO: moveに渡す座標はabsoluteかrelativeか
        PointerDown = 1,
        PointerUp = 2,
        MouseWheel = 3,
    };

    ViewNode();
    explicit ViewNode(Size size);
    virtual ~ViewNode();

    void draw(GraphicsContext& gc) const;

    /**
     * Check nodes and find target node.
     * If appropriate handler is not registered, return nullptr and pass to parent node.
     *
     * @param point target point
     * @param type target event type
     * @return node that need to execute event
     */
    virtual ViewNode* hitTest(const Point& point, EventType type);

    void calculateLayout() const;
    Rectangle getLayout() const;

    virtual void onAttachToWindow(Window* window);
    virtual void onDetachFromWindow();
    void requestRelayout() const;
    void requestRedraw() const;

    void setSize(Size size) const;
    void setWidth(float width) const;
    void setHeight(float height) const;
    void setMargin(Side side, float margin) const;
    void setPadding(Side side, float padding) const;
    void setBorder(Side side, float width, Color color, NodeBorder::LineStyle style);
    void setBackgroundColor(Color color);
    void setOpacity(float opacity);
    void setShadow(float offsetX, float offsetY, Color color, float blurRadius = 0.0f, float spreadRadius = 0.0f);

    YGNodeRef getYogaNode() const;

    bool isFocusable() const;
    void setFocusable(bool isFocusable);

    void setPointerMoveHandler(std::function<void(Point)> func);
    void setPointerDownHandler(std::function<void(Point, MouseButtonType)> func);
    void setPointerUpHandler(std::function<void(Point, MouseButtonType)> func);
    void setPointerEnterHandler(std::function<void(Point)> func);
    void setPointerLeaveHandler(std::function<void(Point)> func);
    void setMouseWheelHandler(std::function<void(Point, int)> func);

    void triggerPointerMoveHandler(Point point) const;
    void triggerPointerDownHandler(Point point, MouseButtonType type) const;
    void triggerPointerUpHandler(Point point, MouseButtonType type) const;
    void triggerPointerEnterHandler(Point point) const;
    void triggerPointerLeaveHandler(Point point) const;
    void triggerMouseWheelHandler(Point point, int delta) const;

protected:
    virtual void drawInternal(GraphicsContext& gc) const = 0;
    virtual bool needLayer() const;

    YGNodeRef m_yogaNode;
    Window *m_window = nullptr;

    std::bitset<4> m_enableHandlers{0};

private:
    void drawBorder(GraphicsContext& gc) const;
    void drawBackgroundColor(GraphicsContext& gc) const;
    void drawShadow(GraphicsContext& gc) const;

    std::array<NodeBorder, 4> m_borders;
    std::optional<Color> m_backgroundColor = std::nullopt;
    std::optional<ShadowParams> m_shadow = std::nullopt;

    float m_opacity = 1.0f;
    bool m_isFocusable = false;

    std::function<void(Point)> m_pointerMoveHandler = nullptr;
    std::function<void(Point, MouseButtonType)> m_pointerDownHandler = nullptr;
    std::function<void(Point, MouseButtonType)> m_pointerUpHandler = nullptr;
    std::function<void(Point)> m_pointerEnterHandler = nullptr;
    std::function<void(Point)> m_pointerLeaveHandler = nullptr;
    std::function<void(Point, int)> m_mouseWheelHandler = nullptr;
};
} // karin

#endif //KARIN_GUI_VIEW_NODE_H
