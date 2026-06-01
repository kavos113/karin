#include "d2d_window_surface.h"

#include "d2d_context.h"

#include <iostream>
#include <stdexcept>

namespace karin
{
D2DWindowSurface::D2DWindowSurface(HWND hwnd, Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext)
    : m_hwnd(hwnd)
    , m_deviceContext(std::move(deviceContext))
{
    createSwapChain();
    acquireBackBuffer();
}

void D2DWindowSurface::cleanUp()
{
    m_backBuffer.Reset();
    m_swapChain.Reset();
}

void D2DWindowSurface::present()
{
    if (FAILED(m_swapChain->Present(1, 0)))
    {
        throw std::runtime_error("Failed to present swap chain");
    }
}

void D2DWindowSurface::resize(Size size)
{
    m_backBuffer.Reset();

    if (FAILED(
        m_swapChain->ResizeBuffers(
            2,
            static_cast<UINT>(size.width),
            static_cast<UINT>(size.height),
            DXGI_FORMAT_B8G8R8A8_UNORM,
            0
        )
    ))
    {
        throw std::runtime_error("Failed to resize swap chain buffers");
    }

    acquireBackBuffer();
}

Microsoft::WRL::ComPtr<ID2D1Bitmap> D2DWindowSurface::getTargetBitmap() const
{
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
    HRESULT hr = m_deviceContext->CreateBitmapFromDxgiSurface(
        m_backBuffer.Get(),
        bitmapProperties,
        &bitmap
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D bitmap from DXGI surface");
    }

    return bitmap;
}

void D2DWindowSurface::createSwapChain()
{
    RECT rect;
    if (!GetClientRect(m_hwnd, &rect))
    {
        throw std::runtime_error("Failed to get client rectangle");
    }

    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
        .Width = static_cast<UINT>(rect.right - rect.left),
        .Height = static_cast<UINT>(rect.bottom - rect.top),
        .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
        .SampleDesc = {1, 0}, // No multisampling
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 2,
        .Scaling = DXGI_SCALING_NONE,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        .AlphaMode = DXGI_ALPHA_MODE_IGNORE,
        .Flags = 0
    };

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    if (FAILED(D2DContext::instance().dxgiDevice()->GetAdapter(&adapter)))
    {
        throw std::runtime_error("Failed to get DXGI adapter");
    }

    Microsoft::WRL::ComPtr<IDXGIFactory2> factory;
    if (FAILED(adapter->GetParent(IID_PPV_ARGS(&factory))))
    {
        throw std::runtime_error("Failed to create DXGI factory");
    }

    if (FAILED(
        factory->CreateSwapChainForHwnd(
            D2DContext::instance().dxgiDevice().Get(),
            m_hwnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &m_swapChain
        )
    ))
    {
        throw std::runtime_error("Failed to create swap chain");
    }
}

void D2DWindowSurface::acquireBackBuffer()
{
    m_backBuffer.Reset();
    if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBuffer))))
    {
        throw std::runtime_error("Failed to get back buffer from swap chain");
    }
}
} // karin