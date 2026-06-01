#include "d2d_renderer_impl.h"

#include "d2d_color.h"
#include "d2d_context.h"

#include <iostream>
#include <stdexcept>

namespace karin
{
D2DRendererImpl::D2DRendererImpl(std::unique_ptr<ID2DSurface> surface)
    : m_surface(std::move(surface))
{
    HRESULT hr = D2DContext::instance().device()->CreateDeviceContext(
        D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_deviceContext
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D device context");
    }

    setTargetBitmap();

    m_deviceResources = std::make_unique<D2DDeviceResources>(m_deviceContext);
    m_fontRenderer = std::make_unique<D2DFontRenderer>(m_deviceContext, m_deviceResources.get());
}

void D2DRendererImpl::setTargetBitmap() const
{
    m_deviceContext->SetTarget(m_surface->getTargetBitmap().Get());
}

void D2DRendererImpl::cleanUp()
{
    m_deviceContext.Reset();
    m_surface = nullptr;
}

bool D2DRendererImpl::beginDraw()
{
    m_deviceContext->BeginDraw();
    m_deviceContext->Clear(m_clearColor);

    return true;
}

void D2DRendererImpl::endDraw()
{
    HRESULT hr = m_deviceContext->EndDraw();
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to end D2D drawing");
    }

    m_surface->present();
}

void D2DRendererImpl::resize(Size size)
{
    m_deviceContext->SetTarget(nullptr);
    m_surface->resize(size);
    setTargetBitmap();
}
} // karin
