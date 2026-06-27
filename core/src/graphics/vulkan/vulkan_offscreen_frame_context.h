#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_FRAME_CONTEXT_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_FRAME_CONTEXT_H

#include <memory>
#include <vector>

#include "vulkan_frame_context.h"
#include "vulkan_offscreen_surface.h"

namespace karin
{
class VulkanOffscreenFrameContext : public VulkanFrameContext
{
public:
    VulkanOffscreenFrameContext(std::unique_ptr<VulkanOffscreenSurface> surface);
    ~VulkanOffscreenFrameContext() override = default;

    void cleanup() override;

    std::vector<std::byte> getImageData() const;

private:
    VulkanOffscreenSurface *m_offscreenSurface;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_FRAME_CONTEXT_H
