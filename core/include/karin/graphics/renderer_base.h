#ifndef CORE_KARIN_GRAPHICS_RENDERER_BASE_H
#define CORE_KARIN_GRAPHICS_RENDERER_BASE_H

#include <functional>

#include <karin/common/color/color.h>
#include "graphics_context.h"
#include "image.h"

namespace karin
{
class IRendererImpl;

class RendererBase
{
public:
    virtual ~RendererBase();

    /**
     * Add a rendering command to the renderer.
     *
     * The order of the commands is guaranteed to be executed in the order they are added,
     * but if the accuracy of the order is important, you should use RenderPass class.
     *
     * example:
     * <pre>
     * addDrawCommand([](GraphicsContext &gc) {
     *     gc.drawRect(Rectangle(0, 0, 100, 100));
     * });
     * </pre>
     *
     * @param command rendering command. use GraphicsContext to draw.
     */
    void addDrawCommand(std::function<void(GraphicsContext &)> command);

    void setClearColor(const Color& color);

    Image createImage(const std::string& filePath);
    Image createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height);

    void cleanUp();

protected:
    std::unique_ptr<IRendererImpl> m_impl;

    std::vector<std::function<void(GraphicsContext &)>> m_drawCommands;
};
} // karin

#endif //CORE_KARIN_GRAPHICS_RENDERER_BASE_H
