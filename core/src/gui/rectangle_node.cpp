#include <karin/gui/rectangle_node.h>

namespace karin::gui
{
void RectangleNode::setColor(Color color)
{
    m_color = color;
}

void RectangleNode::drawBackground(GraphicsContext& gc) const
{
    Rectangle layout = getLayout();
    Pattern pattern = SolidColorPattern(m_color);

    gc.fillRect(layout, pattern);
}
} // karin::gui