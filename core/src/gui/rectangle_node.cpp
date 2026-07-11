#include <karin/gui/rectangle_node.h>

namespace karin::gui
{
void RectangleNode::drawBackground(GraphicsContext& gc) const
{
    Rectangle layout = getLayout();
    Pattern pattern = SolidColorPattern(m_color);
    float opacity = m_opacity;

    gc.withSave([&gc, &layout, &pattern, opacity]
    {
        gc.setAlpha(opacity);
        gc.fillRect(layout, pattern);
    });
}
} // karin::gui