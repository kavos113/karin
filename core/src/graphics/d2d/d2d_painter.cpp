#include "d2d_painter.h"

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
D2DPainter::D2DPainter(
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
    D2DDeviceResources* deviceResources
)
    : m_deviceContext(std::move(deviceContext)),
      m_deviceResources(deviceResources)
{
}

void D2DPainter::fillRect(const Rectangle rect, const Pattern& pattern, const GraphicsContext::State& state)
{
    float centerX = rect.pos.x + rect.size.width / 2;
    float centerY = rect.pos.y + rect.size.height / 2;

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
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);
    brush->SetOpacity(state.alpha);
    brush->SetOpacity(state.alpha);

    m_deviceContext->FillRectangle(
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
        brush.Get()
    );

    if (hasClip)
    {
        m_deviceContext->PopAxisAlignedClip();
    }
}

void D2DPainter::fillEllipse(
    Point center, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state
)
{
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
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-center.x, -center.y);
    brush->SetTransform(brushTransform);
    brush->SetOpacity(state.alpha);

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
}

void D2DPainter::fillRoundedRect(
    Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state
)
{
    float centerX = rect.pos.x + rect.size.width / 2;
    float centerY = rect.pos.y + rect.size.height / 2;

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
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);
    brush->SetOpacity(state.alpha);

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
}

void D2DPainter::drawLine(
    Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    float centerX = (start.x + end.x) / 2;
    float centerY = (start.y + end.y) / 2;

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
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);
    brush->SetOpacity(state.alpha);

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
}

void D2DPainter::drawRect(
    Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    float centerX = rect.pos.x + rect.size.width / 2;
    float centerY = rect.pos.y + rect.size.height / 2;

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
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);
    brush->SetOpacity(state.alpha);

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
}

void D2DPainter::drawEllipse(
    Point center,
    float radiusX,
    float radiusY,
    const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
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
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-center.x, -center.y);
    brush->SetTransform(brushTransform);
    brush->SetOpacity(state.alpha);

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
}

void D2DPainter::drawRoundedRect(
    Rectangle rect,
    float radiusX,
    float radiusY,
    const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    float centerX = rect.pos.x + rect.size.width / 2;
    float centerY = rect.pos.y + rect.size.height / 2;

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
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_MATRIX_3X2_F brushTransform = D2D1::Matrix3x2F::Translation(-centerX, -centerY);
    brush->SetTransform(brushTransform);
    brush->SetOpacity(state.alpha);

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
}

void D2DPainter::fillPath(const PathImpl& path, const Pattern& pattern, const GraphicsContext::State& state)
{
    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    m_deviceContext->SetTransform(toD2DMatrix(state.transform));

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
}

void D2DPainter::drawPath(
    const PathImpl& path, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    bool hasClip = state.clipRect.has_value();
    if (hasClip)
    {
        D2D1_RECT_F clipRect = toD2DRect(state.clipRect.value());
        m_deviceContext->PushAxisAlignedClip(
            clipRect,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );
    }

    m_deviceContext->SetTransform(toD2DMatrix(state.transform));

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
}

void D2DPainter::drawImage(
    Image image, Rectangle destRect, Rectangle srcRect, float opacity, const GraphicsContext::State& state
)
{
    float centerX = destRect.pos.x + destRect.size.width / 2;
    float centerY = destRect.pos.y + destRect.size.height / 2;

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
    m_deviceContext->SetTransform(toD2DMatrix(state.transform) * transitionMatrix);

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
}

void D2DPainter::pushLayer(Rectangle bounds, float alpha, const GraphicsContext::State& state)
{
    D2D1_MATRIX_3X2_F trans = toD2DMatrix(state.transform);
    trans = trans * D2D1::Matrix3x2F::Translation(bounds.pos.x + bounds.size.width / 2, bounds.pos.y + bounds.size.height / 2);

    Rectangle centerBounds = {
        Point(-bounds.size.width / 2, -bounds.size.height / 2),
        bounds.size
    };
    D2D1_LAYER_PARAMETERS layerParams = {
        // .contentBounds = D2D1::InfiniteRect(),
        .contentBounds = toD2DRect(centerBounds),
        .geometricMask = nullptr,
        .maskAntialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
        .maskTransform = D2D1::Matrix3x2F::Identity(),
        .opacity = alpha * state.alpha,
        .opacityBrush = nullptr,
        .layerOptions = D2D1_LAYER_OPTIONS_NONE
    };

    m_deviceContext->SetTransform(trans);
    m_deviceContext->PushLayer(&layerParams, nullptr);
}

void D2DPainter::popLayer()
{
    m_deviceContext->PopLayer();
    m_deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}
} // karin
