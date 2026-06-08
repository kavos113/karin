#include "d2d_canvas.h"

#include "d2d_geometry.h"

#include <algorithm>
#include <stdexcept>
#include <d2d/matrix_converter.h>

namespace
{
D2D1_RECT_F applyTransition(const D2D1_RECT_F& rect, float dx, float dy)
{
    return D2D1::RectF(
        rect.left + dx,
        rect.top + dy,
        rect.right + dx,
        rect.bottom + dy
    );
}
}

namespace karin
{
D2DCanvas::D2DCanvas(
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
    D2DDeviceResources* deviceResources
)
    : m_deviceContext(std::move(deviceContext)),
      m_deviceResources(deviceResources)
{
}

void D2DCanvas::fillRect(const Rectangle rect, const Pattern& pattern, const GraphicsContext::State& state)
{
    float centerX = rect.pos.x + rect.size.width / 2;
    float centerY = rect.pos.y + rect.size.height / 2;

    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(centerX, centerY);
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);

    m_deviceContext->FillRectangle(
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
        brush.Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    brush->SetTransform(D2D1::Matrix3x2F::Identity());

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::fillEllipse(
    Point center, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(center.x, center.y);
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-center.x, -center.y);
    brush->SetTransform(brushTransform);

    D2D1_ELLIPSE ellipse = {
        D2D1::Point2F(0.0f, 0.0f),
        radiusX, radiusY
    };
    m_deviceContext->FillEllipse(
        ellipse,
        brush.Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    brush->SetTransform(D2D1::Matrix3x2F::Identity());

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::fillRoundedRect(
    Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state
)
{
    float centerX = rect.pos.x + rect.size.width / 2;
    float centerY = rect.pos.y + rect.size.height / 2;

    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(centerX, centerY);
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);

    D2D1_ROUNDED_RECT roundedRect = {
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
        std::max(0.0f, radiusX),
        std::max(0.0f, radiusY)
    };
    m_deviceContext->FillRoundedRectangle(
        roundedRect,
        brush.Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    brush->SetTransform(D2D1::Matrix3x2F::Identity());

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::drawLine(
    Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    float centerX = (start.x + end.x) / 2;
    float centerY = (start.y + end.y) / 2;

    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(centerX, centerY);
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);

    m_deviceContext->DrawLine(
        D2D1::Point2F(start.x - centerX, start.y - centerY),
        D2D1::Point2F(end.x - centerX, end.y - centerY),
        brush.Get(),
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    brush->SetTransform(D2D1::Matrix3x2F::Identity());

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::drawRect(
    Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    float centerX = rect.pos.x + rect.size.width / 2;
    float centerY = rect.pos.y + rect.size.height / 2;

    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(centerX, centerY);
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);

    m_deviceContext->DrawRectangle(
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
        brush.Get(),
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    brush->SetTransform(D2D1::Matrix3x2F::Identity());

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::drawEllipse(
    Point center,
    float radiusX,
    float radiusY,
    const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(center.x, center.y);
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-center.x, -center.y);
    brush->SetTransform(brushTransform);

    D2D1_ELLIPSE ellipse = {
        D2D1::Point2F(0.0f, 0.0f),
        radiusX, radiusY
    };
    m_deviceContext->DrawEllipse(
        ellipse,
        brush.Get(),
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    brush->SetTransform(D2D1::Matrix3x2F::Identity());

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::drawRoundedRect(
    Rectangle rect,
    float radiusX,
    float radiusY,
    const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    float centerX = rect.pos.x + rect.size.width / 2;
    float centerY = rect.pos.y + rect.size.height / 2;

    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(centerX, centerY);
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);

    D2D1_ROUNDED_RECT roundedRect = {
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
        std::max(0.0f, radiusX),
        std::max(0.0f, radiusY)
    };
    m_deviceContext->DrawRoundedRectangle(
        roundedRect,
        brush.Get(),
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    brush->SetTransform(D2D1::Matrix3x2F::Identity());

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::fillPath(const PathImpl& path, const Pattern& pattern, const GraphicsContext::State& state)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * oldTransform);

    auto geometry = m_deviceResources->pathGeometry(path);
    if (!geometry)
    {
        throw std::runtime_error("Failed to get path geometry");
    }

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->FillGeometry(
        geometry.Get(),
        brush.Get(),
        nullptr
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::drawPath(
    const PathImpl& path, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * oldTransform);

    auto geometry = m_deviceResources->pathGeometry(path);
    if (!geometry)
    {
        throw std::runtime_error("Failed to get path geometry");
    }

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawGeometry(
        geometry.Get(),
        brush.Get(),
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    m_deviceContext->SetTransform(oldTransform);
}

void D2DCanvas::drawImage(
    Image image, Rectangle destRect, Rectangle srcRect, float opacity, const GraphicsContext::State& state
)
{
    float centerX = destRect.pos.x + destRect.size.width / 2;
    float centerY = destRect.pos.y + destRect.size.height / 2;

    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(centerX, centerY);
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix * oldTransform);

    auto bitmap = m_deviceResources->bitmap(image);
    if (!bitmap)
    {
        throw std::runtime_error("Failed to get bitmap for image");
    }

    D2D1_RECT_F* srcRectPtr = nullptr;
    if (srcRect != Rectangle())
    {
        static D2D1_RECT_F d2dSrcRect;
        d2dSrcRect = toD2DRect(srcRect);
        srcRectPtr = &d2dSrcRect;
    }

    m_deviceContext->DrawBitmap(
        bitmap.Get(),
        D2D1::RectF(-destRect.size.width / 2.0f, -destRect.size.height / 2.0f, destRect.size.width / 2.0f, destRect.size.height / 2.0f),
        opacity,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        srcRectPtr
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }

    m_deviceContext->SetTransform(oldTransform);
}
} // karin
