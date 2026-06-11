#ifndef SRC_GRAPHICS_RESOURCES_PLATFORM_H
#define SRC_GRAPHICS_RESOURCES_PLATFORM_H

#include <memory>

#include <karin/system/window.h>
#include "offscreen_renderer_impl.h"
#include "renderer_impl.h"
#include "painter.h"

#ifdef KARIN_PLATFORM_DIRECTX
#include "d2d/d2d_renderer_impl.h"
#include "d2d/d2d_painter.h"
#include "d2d/d2d_window_surface.h"
#include "d2d/d2d_offscreen_surface.h"
#include "d2d/d2d_offscreen_renderer_impl.h"
#elifdef KARIN_PLATFORM_VULKAN
#include "vulkan/vulkan_renderer_impl.h"
#include "vulkan/vulkan_window_surface.h"
#include "vulkan/vulkan_offscreen_surface.h"
#include "vulkan/vulkan_offscreen_renderer_impl.h"
#include "vulkan/vulkan_painter.h"
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
    auto surface = std::make_unique<VulkanWindowSurface>(handle);
    return std::make_unique<VulkanRendererImpl>(std::move(surface));
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
#elifdef KARIN_PLATFORM_VULKAN
    auto surface = std::make_unique<VulkanOffscreenSurface>(size);
    auto renderer = std::make_unique<VulkanOffscreenRendererImpl>(std::move(surface));

    IOffscreenRendererImpl *offscreenRendererImpl = renderer.get();
    return {
        .rendererImpl = std::move(renderer),
        .offscreenRendererImpl = offscreenRendererImpl
    };
#endif
}

inline std::unique_ptr<IPainter> createPainter(IRendererImpl* impl)
{
#ifdef KARIN_PLATFORM_DIRECTX
    auto d2dImpl = dynamic_cast<D2DRendererImpl*>(impl);
    if (d2dImpl == nullptr)
    {
        throw std::runtime_error("Invalid renderer implementation for DirectX canvas");
    }

    return std::make_unique<D2DPainter>(
        d2dImpl->deviceContext(),
        d2dImpl->deviceResources()
    );
#elifdef KARIN_PLATFORM_VULKAN
    auto vulkanImpl = dynamic_cast<VulkanRendererImpl*>(impl);
    if (vulkanImpl == nullptr)
    {
        throw std::runtime_error("Invalid renderer implementation for Vulkan canvas");
    }

    return std::make_unique<VulkanPainter>(vulkanImpl);
#endif

    return nullptr;
}
}

#endif //SRC_GRAPHICS_RESOURCES_PLATFORM_H