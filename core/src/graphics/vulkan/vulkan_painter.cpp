#include "vulkan_painter.h"

#include "glm_geometry.h"
#include "vulkan_renderer_impl.h"
#include "vulkan_tessellator.h"
#include "shaders/push_constants.slang"

#include <karin/common/color/color.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/stroke_style.h>

#include <cmath>
#include <iostream>
#include <numbers>
#include <stdexcept>
#include <variant>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
using namespace karin;

FragPushConstants createFragPushConstantData(const Pattern& pattern, float alpha)
{
    return std::visit(
        [alpha]<typename T0>(const T0& p) -> FragPushConstants
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, SolidColorPattern>)
            {
                Color color = p.color();
                return FragPushConstants{
                    .color = {color.r, color.g, color.b, color.a},
                    .patternType = static_cast<uint32_t>(PatternType::SolidColor),
                    .patternParams = {alpha, 0.0f, 0.0f, 0.0f}
                };
            }
            else if constexpr (std::is_same_v<T, LinearGradientPattern>)
            {
                return FragPushConstants{
                    .color = {p.start.x, p.start.y, p.end.x, p.end.y},
                    .patternType = static_cast<uint32_t>(PatternType::LinearGradient),
                    .patternParams = {alpha, 0.0f, 0.0f, 0.0f}
                };
            }
            else if constexpr (std::is_same_v<T, RadialGradientPattern>)
            {
                return FragPushConstants{
                    .color = {p.center.x, p.center.y, p.offset.x, p.offset.y},
                    .patternType = static_cast<uint32_t>(PatternType::RadialGradient),
                    .patternParams = {alpha, p.radiusX, p.radiusY, 0.0f},
                };
            }
            else if constexpr (std::is_same_v<T, ImagePattern>)
            {
                return FragPushConstants{
                    .color = {p.offset.x, p.offset.y, p.scaleX, p.scaleY},
                    .patternType = static_cast<uint32_t>(PatternType::Image),
                    .patternParams = {alpha, p.image.width(), p.image.height(), 1.0f}
                };
            }
            else
            {
                throw std::runtime_error("Unsupported pattern type");
            }
        }, pattern
    );
}

VertexPushConstants createVertexPushConstantData(const GraphicsContext::State& state, const Point& position)
{
    const float *data = state.transform.data();
    glm::mat4 trans = glm::mat4(
        data[0], data[1], 0.0f, 0.0f,
        data[3], data[4], 0.0f, 0.0f,
        0.0f,    0.0f,    1.0f, 0.0f,
        data[6], data[7], 0.0f, 1.0f
    );

    glm::mat4 translateMatrix = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(position.x, position.y, 0.0f)
    );
    return VertexPushConstants{
        .model = translateMatrix * trans
    };
}
}

namespace karin
{
VulkanPainter::VulkanPainter(VulkanRendererImpl* renderer)
    : m_renderer(renderer)
{
}

void VulkanPainter::fillRect(Rectangle rect, const Pattern& pattern, const GraphicsContext::State& state)
{
    std::vector<VulkanPipeline::Vertex> vertices = {
        {
            .pos = {-rect.size.width / 2.0f, -rect.size.height / 2.0f},
            .uv = {-1.0f, -1.0f},
        },
        {
            .pos = {rect.size.width / 2.0f, -rect.size.height / 2.0f},
            .uv = {1.0f, -1.0f},
        },
        {
            .pos = {rect.size.width / 2.0f, rect.size.height / 2.0f},
            .uv = {1.0f, 1.0f},
        },
        {
            .pos = { -rect.size.width / 2.0f, rect.size.height / 2.0f},
            .uv = {-1.0f, 1.0f},
        }
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    FragPushConstants fragData = createFragPushConstantData(pattern, state.alpha);
    fragData.shapeParams = {rect.size.width / 2.0f, rect.size.height * 2.0f, 0.0f, 0.0f};
    fragData.shapeType = static_cast<uint32_t>(ShapeType::Rectangle);

    m_renderer->addCommand(
        vertices, indices,
        fragData,
        createVertexPushConstantData(state, Point(
                                         rect.pos.x + rect.size.width / 2.0f,
                                         rect.pos.y + rect.size.height / 2.0f
                                     )),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::fillEllipse(
    Point center, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state
)
{
    std::vector<VulkanPipeline::Vertex> vertices = {
        {
            .pos = {-radiusX, -radiusY},
            .uv = {-1.0f, -1.0f},
        },
        {
            .pos = {radiusX, -radiusY},
            .uv = {1.0f, -1.0f},
        },
        {
            .pos = {radiusX, radiusY},
            .uv = {1.0f, 1.0f},
        },
        {
            .pos = { -radiusX, radiusY},
            .uv = {-1.0f, 1.0f},
        }
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    auto fragData = createFragPushConstantData(pattern, state.alpha);
    fragData.shapeParams = {radiusX, radiusY, 0.0f, 0.0f};
    fragData.shapeType = static_cast<uint32_t>(ShapeType::Ellipse);

    m_renderer->addCommand(
        vertices, indices,
        fragData,
        createVertexPushConstantData(state, center),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::fillRoundedRect(
    Rectangle rect, float radiusX, float radiusY, const Pattern& pattern, const GraphicsContext::State& state
)
{
    std::vector<VulkanPipeline::Vertex> vertices = {
        {
            .pos = {-rect.size.width / 2.0f, -rect.size.height / 2.0f},
            .uv = {-1.0f, -1.0f},
        },
        {
            .pos = {rect.size.width / 2.0f, -rect.size.height / 2.0f},
            .uv = {1.0f, -1.0f},
        },
        {
            .pos = {rect.size.width / 2.0f, rect.size.height / 2.0f},
            .uv = {1.0f, 1.0f},
        },
        {
            .pos = { -rect.size.width / 2.0f, rect.size.height / 2.0f},
            .uv = {-1.0f, 1.0f},
        }
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    auto fragData = createFragPushConstantData(pattern, state.alpha);
    fragData.shapeParams = {rect.size.width / 2.0f, rect.size.height / 2.0f, radiusX, radiusY};
    fragData.shapeType = static_cast<uint32_t>(ShapeType::RoundedRectangle);

    m_renderer->addCommand(
        vertices, indices,
        fragData,
        createVertexPushConstantData(state, Point(
                                         rect.pos.x + rect.size.width / 2.0f,
                                         rect.pos.y + rect.size.height / 2.0f
                                     )),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::drawLine(
    Point start, Point end, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    VulkanTessellator::addLine(
        Point(start.x - (start.x + end.x) / 2, start.y - (start.y + end.y) / 2),
        Point(end.x - (start.x + end.x) / 2, end.y - (start.y + end.y) / 2),
        strokeStyle, vertices, indices
    );

    m_renderer->addCommand(
        vertices, indices,
        createFragPushConstantData(pattern, state.alpha),
        createVertexPushConstantData(state, Point(
                                         (start.x + end.x) / 2.0f,
                                         (start.y + end.y) / 2.0f
                                     )),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::drawRect(
    Rectangle rect, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;
    float dashOffset = VulkanTessellator::addLine(
        Point(-rect.size.width / 2.0f, -rect.size.height / 2.0f),
        Point(rect.size.width / 2.0f, -rect.size.height / 2.0f),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    dashOffset = VulkanTessellator::addLine(
        Point(rect.size.width / 2.0f, -rect.size.height / 2.0f),
        Point(rect.size.width / 2.0f, rect.size.height / 2.0f),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    dashOffset = VulkanTessellator::addLine(
        Point(rect.size.width / 2.0f, rect.size.height / 2.0f),
        Point(-rect.size.width / 2.0f, rect.size.height / 2.0f),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    VulkanTessellator::addLine(
        Point(-rect.size.width / 2.0f, rect.size.height / 2.0f),
        Point(-rect.size.width / 2.0f, -rect.size.height / 2.0f),
        style,
        vertices,
        indices
    );

    m_renderer->addCommand(
        vertices, indices,
        createFragPushConstantData(pattern, state.alpha),
        createVertexPushConstantData(state, Point(
                                         rect.pos.x + rect.size.width / 2.0f,
                                         rect.pos.y + rect.size.height / 2.0f
                                     )),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::drawEllipse(
    Point center, float radiusX, float radiusY, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;

    VulkanTessellator::addArc(
        Point(0.0f, 0.0f),
        radiusX,
        radiusY,
        0.0f,
        2.0f * std::numbers::pi,
        false,
        style,
        vertices,
        indices
    );

    m_renderer->addCommand(
        vertices, indices,
        createFragPushConstantData(pattern, state.alpha),
        createVertexPushConstantData(state, center),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::drawRoundedRect(
    Rectangle rect,
    float radiusX,
    float radiusY,
    const Pattern& pattern,
    const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;

    float offset = VulkanTessellator::addArc(
        Point(-rect.size.width / 2.0f + radiusX, -rect.size.height / 2.0f + radiusY),
        radiusX,
        radiusY,
        std::numbers::pi,
        0.5f * std::numbers::pi,
        true,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = VulkanTessellator::addLine(
        Point(-rect.size.width / 2.0f + radiusX, -rect.size.height / 2.0f),
        Point(rect.size.width / 2.0f - radiusX, -rect.size.height / 2.0f),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = VulkanTessellator::addArc(
        Point(rect.size.width / 2.0f - radiusX, -rect.size.height / 2.0f + radiusY),
        radiusX,
        radiusY,
        0.5f * std::numbers::pi,
        0.0f,
        true,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = VulkanTessellator::addLine(
        Point(rect.size.width / 2.0f, -rect.size.height / 2.0f + radiusY),
        Point(rect.size.width / 2.0f, rect.size.height / 2.0f - radiusY),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = VulkanTessellator::addArc(
        Point(rect.size.width / 2.0f - radiusX, rect.size.height / 2.0f - radiusY),
        radiusX,
        radiusY,
        0.0f,
        1.5f * std::numbers::pi,
        true,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = VulkanTessellator::addLine(
        Point(rect.size.width / 2.0f - radiusX, rect.size.height / 2.0f),
        Point(-rect.size.width / 2.0f + radiusX, rect.size.height / 2.0f),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = VulkanTessellator::addArc(
        Point(-rect.size.width / 2.0f + radiusX, rect.size.height / 2.0f - radiusY),
        radiusX,
        radiusY,
        1.5f * std::numbers::pi,
        std::numbers::pi,
        true,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    VulkanTessellator::addLine(
        Point(-rect.size.width / 2.0f, rect.size.height / 2.0f - radiusY),
        Point(-rect.size.width / 2.0f, -rect.size.height / 2.0f + radiusY),
        style,
        vertices,
        indices
    );

    m_renderer->addCommand(
        vertices, indices,
        createFragPushConstantData(pattern, state.alpha),
        createVertexPushConstantData(state, Point(
                                         rect.pos.x + rect.size.width / 2.0f,
                                         rect.pos.y + rect.size.height / 2.0f
                                     )),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::fillPath(const PathImpl& path, const Pattern& pattern, const GraphicsContext::State& state)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    std::vector<Point> polygonPoints;

    auto commands = path.commands();

    for (const auto& command : commands)
    {
        std::visit(
            [&polygonPoints]<typename T0>(const T0& args)
            {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, PathImpl::LineArgs>)
                {
                    if (polygonPoints.empty() || polygonPoints.back() != args.end)
                    {
                        polygonPoints.push_back(args.end);
                    }
                }
                else if constexpr (std::is_same_v<T, PathImpl::ArcArgs>)
                {
                    bool isClockwise = args.isSmallArc
                                           ? (args.endAngle < args.startAngle)
                                           : (args.endAngle > args.startAngle);

                    auto arcPoints = VulkanTessellator::splitArc(
                        args.center,
                        args.radiusX,
                        args.radiusY,
                        args.startAngle,
                        args.endAngle,
                        isClockwise
                    );

                    if (arcPoints.size() < 2)
                    {
                        return; // No points to add
                    }

                    for (int i = 1; i < arcPoints.size(); ++i)
                    {
                        if (polygonPoints.empty() || polygonPoints.back() != arcPoints[i])
                        {
                            polygonPoints.push_back(arcPoints[i]);
                        }
                    }
                }
            },
            command
        );
    }

    for (auto point : polygonPoints)
    {
        vertices.push_back(
            {
                .pos = {point.x, point.y},
                .uv = {-1.0f, -1.0f} // UV coordinates are not used for fill
            }
        );
    }

    auto triangles = VulkanTessellator::triangulate(polygonPoints);
    for (size_t i = 0; i < triangles.size(); i += 3)
    {
        indices.push_back(triangles[i]);
        indices.push_back(triangles[i + 1]);
        indices.push_back(triangles[i + 2]);
    }

    m_renderer->addCommand(
        vertices, indices,
        createFragPushConstantData(pattern, state.alpha),
        createVertexPushConstantData(state, Point(0.0f, 0.0f)),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::drawPath(
    const PathImpl& path, const Pattern& pattern, const StrokeStyle& strokeStyle, const GraphicsContext::State& state
)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;

    auto commands = path.commands();
    Point currentPoint = path.startPoint();

    for (const auto& command : commands)
    {
        std::visit(
            [&style, &vertices, &indices, &currentPoint]<typename T0>(const T0& args)
            {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, PathImpl::LineArgs>)
                {
                    float offset = VulkanTessellator::addLine(
                        currentPoint,
                        args.end,
                        style,
                        vertices,
                        indices
                    );

                    currentPoint = args.end;
                    style.dash_offset = offset;
                }
                else if constexpr (std::is_same_v<T, PathImpl::ArcArgs>)
                {
                    bool isClockwise = args.isSmallArc
                                           ? (args.endAngle < args.startAngle)
                                           : (args.endAngle > args.startAngle);

                    float offset = VulkanTessellator::addArc(
                        args.center,
                        args.radiusX,
                        args.radiusY,
                        args.startAngle,
                        args.endAngle,
                        isClockwise,
                        style,
                        vertices,
                        indices
                    );
                    style.dash_offset = offset;

                    currentPoint = Point(
                        args.center.x + args.radiusX * std::cos(args.endAngle),
                        args.center.y + args.radiusY * std::sin(-args.endAngle) // bottom is big
                    );
                }
            },
            command
        );
    }

    m_renderer->addCommand(
        vertices, indices,
        createFragPushConstantData(pattern, state.alpha),
        createVertexPushConstantData(state, Point(0.0f, 0.0f)),
        pattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::drawImage(
    Image image, Rectangle destRect, Rectangle srcRect, float opacity, const GraphicsContext::State& state
)
{
    Rectangle normalizedSrcRect{
        srcRect.pos.x / image.width(),
        srcRect.pos.y / image.height(),
        srcRect.size.width / image.width(),
        srcRect.size.height / image.height()
    };

    if (srcRect == Rectangle())
    {
        normalizedSrcRect = Rectangle(0.0f, 0.0f, 1.0f, 1.0f);
    }

    std::vector<VulkanPipeline::Vertex> vertices = {
        {
            .pos = {-destRect.size.width / 2.0f, -destRect.size.height / 2.0f},
            .uv = {normalizedSrcRect.pos.x, normalizedSrcRect.pos.y},
        },
        {
            .pos = {destRect.size.width / 2.0f, -destRect.size.height / 2.0f},
            .uv = {normalizedSrcRect.pos.x + normalizedSrcRect.size.width, normalizedSrcRect.pos.y},
        },
        {
            .pos = {destRect.size.width / 2.0f, destRect.size.height / 2.0f},
            .uv = {
                normalizedSrcRect.pos.x + normalizedSrcRect.size.width,
                normalizedSrcRect.pos.y + normalizedSrcRect.size.height
            },
        },
        {
            .pos = { -destRect.size.width / 2.0f, destRect.size.height / 2.0f},
            .uv = {normalizedSrcRect.pos.x, normalizedSrcRect.pos.y + normalizedSrcRect.size.height},
        }
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    ImagePattern imagePattern{
        .image = image,
        .offset = Point(0.0f, 0.0f),
        .scaleX = normalizedSrcRect.size.width,
        .scaleY = normalizedSrcRect.size.height
    };
    FragPushConstants pushConstants = createFragPushConstantData(imagePattern, state.alpha);
    pushConstants.patternParams.w = 0.0f; // uvmode = 0

    m_renderer->addCommand(
        vertices, indices,
        pushConstants,
        createVertexPushConstantData(state, Point(
                                         destRect.pos.x + destRect.size.width / 2.0f,
                                         destRect.pos.y + destRect.size.height / 2.0f
                                     )),
        imagePattern,
        VulkanRendererImpl::PipelineType::Geometry,
        state.clipRect
    );
}

void VulkanPainter::pushLayer(Rectangle bounds, float alpha, const GraphicsContext::State& state)
{
    // TODO: GraphicsContext::Stateの利用
    float halfWidth = bounds.size.width / 2.0f;
    float halfHeight = bounds.size.height / 2.0f;
    Rectangle center(-halfWidth, -halfHeight, bounds.size.width, bounds.size.height);
    Rectangle rec = applyTransform(state.transform, center);
    rec.pos = rec.pos + Point(bounds.pos.x + halfWidth, bounds.pos.y + halfHeight);

    m_renderer->beginOffscreenLayer(rec, alpha);
}

void VulkanPainter::popLayer()
{
    m_renderer->endOffscreenLayer();
}
} // karin
