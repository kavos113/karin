#include "d2d_offscreen_surface.h"

#include <stdexcept>

namespace karin
{
D2DOffscreenSurface::D2DOffscreenSurface(Size size)
    : m_size(size)
{
}

void D2DOffscreenSurface::cleanUp()
{
    m_bitmap.Reset();
}

void D2DOffscreenSurface::present()
{
}

void D2DOffscreenSurface::resize(Size size)
{
    m_size = size;
    m_bitmap.Reset();
    createBitmap();
}

Microsoft::WRL::ComPtr<ID2D1Bitmap> D2DOffscreenSurface::getTargetBitmap() const
{
    return m_bitmap;
}

void D2DOffscreenSurface::setDeviceContext(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext)
{
    m_deviceContext = deviceContext;

    createBitmap();
}

std::vector<std::byte> D2DOffscreenSurface::getImageData() const
{
    D2D1_SIZE_U bitmapSize = m_bitmap->GetPixelSize();
    std::vector<std::byte> imageData(bitmapSize.width * bitmapSize.height * 4);

    D2D1_PIXEL_FORMAT pixelFormat = m_bitmap->GetPixelFormat();
    D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
        pixelFormat
    );

    Microsoft::WRL::ComPtr<ID2D1Bitmap1> stagingBitmap;
    HRESULT hr = m_deviceContext->CreateBitmap(
        bitmapSize,
        nullptr,
        0,
        &props,
        &stagingBitmap
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create staging bitmap for reading");
    }

    D2D1_POINT_2U destPoint = {0, 0};
    D2D1_RECT_U srcRect = {0, 0, bitmapSize.width, bitmapSize.height};
    stagingBitmap->CopyFromBitmap(&destPoint, m_bitmap.Get(), &srcRect);

    D2D1_MAPPED_RECT mappedRect;
    hr = stagingBitmap->Map(D2D1_MAP_OPTIONS_READ, &mappedRect);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to map staging bitmap for reading");
    }

    for (uint32_t y = 0; y < bitmapSize.height; y++)
    {
        std::byte *srcRow = reinterpret_cast<std::byte*>(mappedRect.bits + y * mappedRect.pitch);
        std::byte *destRow = imageData.data() + y * bitmapSize.width * 4;
        memcpy(destRow, srcRow, bitmapSize.width * 4);
    }

    stagingBitmap->Unmap();

    return imageData;
}

void D2DOffscreenSurface::createBitmap()
{
    HRESULT hr = m_deviceContext->CreateBitmap(
        D2D1::SizeU(m_size.width, m_size.height),
        nullptr,
        0,
        bitmapProperties,
        &m_bitmap
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create offscreen bitmap");
    }
}
} // karin