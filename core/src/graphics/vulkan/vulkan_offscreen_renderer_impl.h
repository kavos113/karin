#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_RENDERER_IMPL_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_RENDERER_IMPL_H

#include <cstddef>

#include <memory>
#include <vector>

#include <offscreen_renderer_impl.h>
#include "vulkan_offscreen_frame_context.h"
#include "vulkan_renderer_impl.h"

namespace karin
{
class VulkanOffscreenRendererImpl : public VulkanRendererImpl, public IOffscreenRendererImpl
{
public:
    VulkanOffscreenRendererImpl(std::unique_ptr<VulkanOffscreenFrameContext> surface);
    ~VulkanOffscreenRendererImpl() override = default;

    void cleanUp() override;

    std::vector<std::byte> getImageData() const override;

private:
    VulkanOffscreenFrameContext *m_offscreenContext;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_OFFSCREEN_RENDERER_IMPL_H
