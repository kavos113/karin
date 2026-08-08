#include <karin/gui/view_node.h>

#include <karin/graphics/stroke_style.h>
#include <karin/gui/window.h>

namespace
{
using namespace karin::gui;
using namespace karin;

YGEdge toYogaEdge(ViewNode::Side side)
{
    switch (side)
    {
    case ViewNode::Side::Left:
        return YGEdgeLeft;
    case ViewNode::Side::Top:
        return YGEdgeTop;
    case ViewNode::Side::Right:
        return YGEdgeRight;
    case ViewNode::Side::Bottom:
        return YGEdgeBottom;
    case ViewNode::Side::Horizontal:
        return YGEdgeHorizontal;
    case ViewNode::Side::Vertical:
        return YGEdgeVertical;
    case ViewNode::Side::All:
        return YGEdgeAll;
    default:
        return YGEdgeAll; // Default to All if unknown
    }
}

StrokeStyle toStrokeStyle(NodeBorder::LineStyle style, float width)
{
    StrokeStyle strokeStyle;
    strokeStyle.width = width;

    switch (style)
    {
    case NodeBorder::LineStyle::None:
        strokeStyle.dash_pattern.clear();
        break;
    case NodeBorder::LineStyle::Solid:
        strokeStyle.dash_pattern.clear();
        break;
    case NodeBorder::LineStyle::Dashed:
        strokeStyle.dash_pattern = { 4.0f * width, 4.0f * width };
        break;
    case NodeBorder::LineStyle::Dotted:
        strokeStyle.dash_pattern = { width, width };
        strokeStyle.start_cap_style = StrokeStyle::CapStyle::Round;
        strokeStyle.end_cap_style = StrokeStyle::CapStyle::Round;
        strokeStyle.dash_cap_style = StrokeStyle::CapStyle::Round;
        break;
    }

    return strokeStyle;
}
}

namespace karin::gui
{
ViewNode::ViewNode()
{
    m_yogaNode = YGNodeNew();

    m_borders.fill({ 0.0f, Color(), NodeBorder::LineStyle::None });
}

ViewNode::ViewNode(Size size)
{
    m_yogaNode = YGNodeNew();
    YGNodeStyleSetWidth(m_yogaNode, size.width);
    YGNodeStyleSetHeight(m_yogaNode, size.height);

    m_borders.fill({ 0.0f, Color(), NodeBorder::LineStyle::None });
}

ViewNode::~ViewNode()
{
    YGNodeFree(m_yogaNode);
}

void ViewNode::draw(GraphicsContext& gc) const
{
    if (needLayer())
    {
        Rectangle layout = getLayout();
        gc.withLayer(layout, m_opacity, [&gc, this]
        {
            gc.withSave([&gc, this]
            {
                gc.setAlpha(m_opacity);

                drawShadow(gc);
                drawBackgroundColor(gc);
                gc.withSave([&gc, this]
                {
                    drawInternal(gc);
                });
                drawBorder(gc);
            });
        });
    }
    else
    {
        gc.withSave([&gc, this]
        {
            gc.setAlpha(m_opacity);

            drawShadow(gc);
            drawBackgroundColor(gc);
            gc.withSave([&gc, this]
            {
                drawInternal(gc);
            });
            drawBorder(gc);
        });
    }
}

void ViewNode::calculateLayout() const
{
    YGNodeCalculateLayout(m_yogaNode, YGUndefined, YGUndefined, YGDirectionLTR);
}

Rectangle ViewNode::getLayout() const
{
    return {
        YGNodeLayoutGetLeft(m_yogaNode),
        YGNodeLayoutGetTop(m_yogaNode),
        YGNodeLayoutGetWidth(m_yogaNode),
        YGNodeLayoutGetHeight(m_yogaNode)
    };
}

void ViewNode::onAttachToWindow(Window* window)
{
    m_window = window;
}

void ViewNode::onDetachFromWindow()
{
    m_window = nullptr;
}

void ViewNode::requestRelayout() const
{
    if (m_window)
    {
        m_window->requestRelayout();
    }
}

void ViewNode::requestRedraw() const
{
    if (m_window)
    {
        m_window->requestRedraw();
    }
}

void ViewNode::setSize(Size size) const
{
    YGNodeStyleSetWidth(m_yogaNode, size.width);
    YGNodeStyleSetHeight(m_yogaNode, size.height);
}

void ViewNode::setWidth(float width) const
{
    YGNodeStyleSetWidth(m_yogaNode, width);
}

void ViewNode::setHeight(float height) const
{
    YGNodeStyleSetHeight(m_yogaNode, height);
}

void ViewNode::setMargin(Side side, float margin) const
{
    YGNodeStyleSetMargin(m_yogaNode, toYogaEdge(side), margin);
}

void ViewNode::setPadding(Side side, float padding) const
{
    YGNodeStyleSetPadding(m_yogaNode, toYogaEdge(side), padding);
}

void ViewNode::setBorder(Side side, float width, Color color, NodeBorder::LineStyle style)
{
    YGNodeStyleSetBorder(m_yogaNode, toYogaEdge(side), width);

    switch (side)
    {
    case Side::Left:
        m_borders[0] = { width, color, style };
        break;
    case Side::Top:
        m_borders[1] = { width, color, style };
        break;
    case Side::Right:
        m_borders[2] = { width, color, style };
        break;
    case Side::Bottom:
        m_borders[3] = { width, color, style };
        break;
    case Side::Horizontal:
        m_borders[0] = { width, color, style };
        m_borders[2] = { width, color, style };
        break;
    case Side::Vertical:
        m_borders[1] = { width, color, style };
        m_borders[3] = { width, color, style };
        break;
    case Side::All:
        for (auto& border : m_borders)
        {
            border = { width, color, style };
        }
        break;
    }
}

void ViewNode::setBackgroundColor(Color color)
{
    m_backgroundColor = color;
}

void ViewNode::setOpacity(float opacity)
{
    m_opacity = opacity;
}

void ViewNode::setShadow(float offsetX, float offsetY, Color color, float blurRadius, float spreadRadius)
{
    m_shadow = ShadowParams{offsetX, offsetY, color, blurRadius, spreadRadius};
}

YGNodeRef ViewNode::getYogaNode() const
{
    return m_yogaNode;
}

void ViewNode::setPointerMoveHandler(std::function<void(Point)> func)
{
    m_pointerMoveHandler = std::move(func);
    m_enableHandlers.set(static_cast<size_t>(EventType::PointerMove));
}

void ViewNode::setPointerDownHandler(std::function<void(Point, MouseButtonType)> func)
{
    m_pointerDownHandler = std::move(func);
    m_enableHandlers.set(static_cast<size_t>(EventType::PointerClick));
}

void ViewNode::setPointerUpHandler(std::function<void(Point, MouseButtonType)> func)
{
    m_pointerUpHandler = std::move(func);
    m_enableHandlers.set(static_cast<size_t>(EventType::PointerClick));
}

void ViewNode::setPointerEnterHandler(std::function<void(Point)> func)
{
    m_pointerEnterHandler = std::move(func);
    m_enableHandlers.set(static_cast<size_t>(EventType::PointerMove));
}

void ViewNode::setPointerLeaveHandler(std::function<void(Point)> func)
{
    m_pointerLeaveHandler = std::move(func);
    m_enableHandlers.set(static_cast<size_t>(EventType::PointerMove));
}

void ViewNode::setMouseWheelHandler(std::function<void(Point, int)> func)
{
    m_mouseWheelHandler = std::move(func);
    m_enableHandlers.set(static_cast<size_t>(EventType::MouseWheel));
}

void ViewNode::setKeyHandler(std::function<void(KeyEvent)> func)
{
    m_keyHandler = std::move(func);
}

void ViewNode::setKeyTypeHandler(std::function<void(std::string)> func)
{
    m_keyTypeHandler = std::move(func);
}

void ViewNode::triggerPointerMoveHandler(Point point) const
{
    if (m_pointerMoveHandler)
    {
        m_pointerMoveHandler(point);
    }
}

void ViewNode::triggerPointerDownHandler(Point point, MouseButtonType type) const
{
    if (m_pointerDownHandler)
    {
        m_pointerDownHandler(point, type);
    }
}

void ViewNode::triggerPointerUpHandler(Point point, MouseButtonType type) const
{
    if (m_pointerUpHandler)
    {
        m_pointerUpHandler(point, type);
    }
}

void ViewNode::triggerPointerEnterHandler(Point point) const
{
    if (m_pointerEnterHandler)
    {
        m_pointerEnterHandler(point);
    }
}

void ViewNode::triggerPointerLeaveHandler(Point point) const
{
    if (m_pointerLeaveHandler)
    {
        m_pointerLeaveHandler(point);
    }
}

void ViewNode::triggerMouseWheelHandler(Point point, int delta) const
{
    if (m_mouseWheelHandler)
    {
        m_mouseWheelHandler(point, delta);
    }
}

void ViewNode::triggerKeyHandler(KeyEvent key) const
{
    if (m_keyHandler)
    {
        m_keyHandler(key);
    }
}

void ViewNode::triggerKeyTypeHandler(std::string c) const
{
    if (m_keyTypeHandler)
    {
        m_keyTypeHandler(c);
    }
}

bool ViewNode::isFocusable() const
{
    return m_isFocusable;
}

void ViewNode::setFocusable(bool isFocusable)
{
    m_isFocusable = isFocusable;
}

bool ViewNode::needLayer() const
{
    return m_opacity < 1.0f;
}

void ViewNode::drawBorder(GraphicsContext& gc) const
{
    Rectangle layout = getLayout();

    Point topLeft = { layout.pos.x, layout.pos.y };
    Point topRight = { layout.pos.x + layout.size.width, layout.pos.y };
    Point bottomLeft = { layout.pos.x, layout.pos.y + layout.size.height };
    Point bottomRight = { layout.pos.x + layout.size.width, layout.pos.y + layout.size.height };

    if (m_borders[0].style != NodeBorder::LineStyle::None)
    {
        Pattern pattern = SolidColorPattern(m_borders[0].color);
        gc.drawLine(
            topLeft,
            bottomLeft,
            pattern,
            toStrokeStyle(m_borders[0].style, m_borders[0].width)
        );
    }

    if (m_borders[1].style != NodeBorder::LineStyle::None)
    {
        Pattern pattern = SolidColorPattern(m_borders[1].color);
        gc.drawLine(
            topLeft,
            topRight,
            pattern,
            toStrokeStyle(m_borders[1].style, m_borders[1].width)
        );
    }

    if (m_borders[2].style != NodeBorder::LineStyle::None)
    {
        Pattern pattern = SolidColorPattern(m_borders[2].color);
        gc.drawLine(
            topRight,
            bottomRight,
            pattern,
            toStrokeStyle(m_borders[2].style, m_borders[2].width)
        );
    }

    if (m_borders[3].style != NodeBorder::LineStyle::None)
    {
        Pattern pattern = SolidColorPattern(m_borders[3].color);
        gc.drawLine(
            bottomLeft,
            bottomRight,
            pattern,
            toStrokeStyle(m_borders[3].style, m_borders[3].width)
        );
    }
}

void ViewNode::drawBackgroundColor(GraphicsContext& gc) const
{
    if (m_backgroundColor.has_value())
    {
        Rectangle layout = getLayout();
        Pattern pattern = SolidColorPattern(m_backgroundColor.value());

        gc.fillRect(layout, pattern);
    }
}

void ViewNode::drawShadow(GraphicsContext& gc) const
{
    if (m_shadow.has_value())
    {
        ShadowParams param = m_shadow.value();

        Rectangle layout = getLayout();
        layout.pos = Point(layout.pos.x + param.offsetX, layout.pos.y + param.offsetY);

        gc.fillBoxShadow(layout, param.color, param.blurRadius, param.spreadRadius);
    }
}

ViewNode* ViewNode::hitTest(const Point& point, EventType type)
{
    float width = YGNodeLayoutGetWidth(m_yogaNode);
    float height = YGNodeLayoutGetHeight(m_yogaNode);

    if (point.x < 0 || point.y < 0 || point.x > width || point.y > height)
    {
        return nullptr;
    }

    if (!m_enableHandlers[static_cast<size_t>(type)])
    {
        return nullptr;
    }

    return this;
}
} // karin