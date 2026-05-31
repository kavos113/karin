#ifndef KARIN_GRAPHICS_GRAPHICS_RENDERER_H
#define KARIN_GRAPHICS_GRAPHICS_RENDERER_H

#include <karin/system/window.h>
#include "renderer_base.h"

namespace karin
{
/**
 * Renderer manages window surface(includes swapchain) of low-level graphics API(D2D -> ID2D1GraphicsContext, Vulkan -> VkSurface).
 *
 * It also has a list of draw commands that can be executed in the rendering loop.
 */
class WindowRenderer : public RendererBase
{
public:
    WindowRenderer(Window* window);
    ~WindowRenderer() override;

    /**
     * Apply renderer settings to native window.
     */
    void update() const;

private:
    Window* m_window;
};
} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_RENDERER_H