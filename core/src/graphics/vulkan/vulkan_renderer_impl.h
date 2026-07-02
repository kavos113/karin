#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H

#include <cstddef>
#include <cstdint>

#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>

#include <renderer_impl.h>
#include <font_renderer_impl.h>
#include <karin/common/geometry/size.h>
#include <karin/common/color/color.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/image.h>
#include "vulkan_device_resources.h"
#include "vulkan_pipeline.h"
#include "vulkan_font_renderer.h"
#include "vulkan_geometry_buffer.h"
#include "vulkan_view_context.h"
#include "vulkan_frame_context.h"
#include "shaders/push_constants.h"

namespace karin
{
/*
 * Push Constant Layout:
 * | FragPushConstants | VertexPushConstants |
 * more details, see shaders/push_constants.h
 *
 * Descriptor Set Layout:
 * | Projection Matrix | Geometry Resources(gradient LUT / Image) | (Glyph Atlas: text only) |
 */
class VulkanRendererImpl : public IRendererImpl
{
public:
    enum class PipelineType
    {
        Geometry,
        Text,
    };

    VulkanRendererImpl(std::unique_ptr<VulkanFrameContext> frameContext);
    ~VulkanRendererImpl() override = default;

    void cleanUp() override;

    bool beginDraw() override;
    void endDraw() override;
    void resize(Size size) override;

    void setClearColor(const Color& color) override
    {
        m_clearColor = {
            .color = {
                .float32 = {color.r, color.g, color.b, color.a}
            }
        };
    }

    void addCommand(
        const std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices,
        const FragPushConstants& fragData,
        const VertexPushConstants& vertData,
        const Pattern& pattern,
        PipelineType pipelineType,
        std::optional<Rectangle> clipRect = std::nullopt
    );

    void beginOffscreenLayer(const Rectangle& bounds, float alpha);
    void endOffscreenLayer();

    void startResizing() override
    {
        m_frameContext->startResizing();
    }

    void finishResizing() override
    {
        m_frameContext->finishResizing();
    }

    Image createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height) override
    {
        return m_deviceResources->createImage(data, width, height);
    }

    VulkanDeviceResources* deviceResources() const
    {
        return m_deviceResources.get();
    }

    IFontRendererImpl* fontRenderer() override
    {
        return m_fontRenderer.get();
    }

protected:
    std::unique_ptr<VulkanFrameContext> m_frameContext;

private:
    struct DrawCommand
    {
        uint32_t indexCount{};
        uint32_t indexOffset{};
        FragPushConstants fragData;
        VertexPushConstants vertData;
        PipelineType pipelineType;
        std::optional<VkRect2D> scissor;
        std::vector<const VulkanTextureResourceDescriptor *> textureResources;
    };

    // viewport, scissor: if primary batch, fill after begin command buffer
    struct DrawBatch
    {
        bool isOffscreenLayer = false;

        VkViewport viewport;
        VkRect2D scissor;

        // layerID: primary target is 0
        uint16_t layerID = 0;
        VkImageLayout renderTargetImageLayout;
        VkAttachmentLoadOp loadOp;
        VkClearValue clearValue;
        VkRect2D targetRect;

        std::vector<DrawCommand> commands;
    };

    struct RenderState
    {
        bool isOffscreenLayer = false;
        Rectangle targetRect;
        uint16_t layerID;
    };
    uint16_t m_lastLayerID = 0;

    void createPipeline();

    std::unordered_map<PipelineType, std::unique_ptr<VulkanPipeline>> m_pipelines;
    std::unique_ptr<VulkanDeviceResources> m_deviceResources;
    std::unique_ptr<VulkanFontRenderer> m_fontRenderer;

    std::unique_ptr<VulkanGeometryBuffer> m_geometryBuffer;
    std::unique_ptr<VulkanViewContext> m_viewContext;

    std::vector<RenderState> m_renderCommandStack;
    std::vector<DrawBatch> m_drawBatches;

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
    static constexpr size_t RENDER_COMMAND_STACK_SIZE = 16;

    VkClearValue m_clearColor = {{1.0f, 1.0f, 1.0f, 1.0f}};
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H