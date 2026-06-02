#ifndef SRC_GRAPHICS_RESOURCES_PLATFORM_H
#define SRC_GRAPHICS_RESOURCES_PLATFORM_H

#include <memory>

#include <karin/system/window.h>
#include "graphics_context_impl.h"
#include "offscreen_renderer_impl.h"
#include "renderer_impl.h"

#ifdef KARIN_PLATFORM_DIRECTX
#include "d2d/d2d_renderer_impl.h"
#include "d2d/d2d_graphics_context_impl.h"
#include "d2d/d2d_window_surface.h"
#include "d2d/d2d_offscreen_surface.h"
#include "d2d/d2d_offscreen_renderer_impl.h"
#elifdef KARIN_PLATFORM_VULKAN
#include "vulkan/vulkan_renderer_impl.h"
#include "vulkan/vulkan_graphics_context_impl.h"
#include "text/font_loader.h"
#endif

namespace karin
{

inline std::unique_ptr<IRendererImpl> createRendererImpl(
    Window::NativeHandle handle
)
{
#ifdef KARIN_PLATFORM_DIRECTX
    auto surface = std::make_unique<D2DWindowSurface>(static_cast<HWND>(handle.hwnd));
    return std::make_unique<D2DRendererImpl>(std::move(surface));
#elifdef KARIN_PLATFORM_VULKAN
    return std::make_unique<VulkanRendererImpl>(handle);
#endif
    return nullptr;
}

struct OffscreenRendererComponents
{
    std::unique_ptr<IRendererImpl> rendererImpl;
    IOffscreenRendererImpl *offscreenRendererImpl;
};

inline OffscreenRendererComponents createOffscreenRendererImpl(Size size)
{
#ifdef KARIN_PLATFORM_DIRECTX
    auto surface = std::make_unique<D2DOffscreenSurface>(size);
    auto renderer = std::make_unique<D2DOffscreenRendererImpl>(std::move(surface));

    IOffscreenRendererImpl *offscreenRendererImpl = renderer.get();
    return {
        .rendererImpl = std::move(renderer),
        .offscreenRendererImpl = offscreenRendererImpl
    };
#endif
}

inline std::unique_ptr<IGraphicsContextImpl> createGraphicsContextImpl(IRendererImpl* impl)
{
#ifdef KARIN_PLATFORM_DIRECTX
    auto d2dImpl = dynamic_cast<D2DRendererImpl*>(impl);
    return std::make_unique<D2DGraphicsContextImpl>(
        d2dImpl->deviceContext(),
        d2dImpl->deviceResources()
    );
#elifdef KARIN_PLATFORM_VULKAN
    return std::make_unique<VulkanGraphicsContextImpl>(
        dynamic_cast<VulkanRendererImpl*>(impl)
    );
#endif

    return nullptr;
}
}

#endif //SRC_GRAPHICS_RESOURCES_PLATFORM_H