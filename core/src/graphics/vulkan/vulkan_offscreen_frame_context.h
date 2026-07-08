#ifndef GRAPHICS_VULKAN_VULKAN_OFFSCREEN_FRAME_CONTEXT_H
#define GRAPHICS_VULKAN_VULKAN_OFFSCREEN_FRAME_CONTEXT_H

#include <memory>
#include <vector>

#include <offscreen_renderer_target.h>
#include "vulkan_frame_context.h"
#include "vulkan_offscreen_surface.h"

namespace karin
{
class VulkanOffscreenFrameContext : public VulkanFrameContext, public IOffscreenRendererTarget
{
public:
    VulkanOffscreenFrameContext(std::unique_ptr<VulkanOffscreenSurface> surface);
    ~VulkanOffscreenFrameContext() override = default;

    void cleanup() override;

    std::vector<std::byte> getImageData() const override;

private:
    VulkanOffscreenSurface *m_offscreenSurface;
};
} // karin

#endif //GRAPHICS_VULKAN_VULKAN_OFFSCREEN_FRAME_CONTEXT_H
