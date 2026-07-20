#ifndef GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H
#define GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H

#ifdef __cplusplus
#include <glm/glm.hpp>
#include <karin/graphics/pattern.h>

namespace karin
{
enum class ShapeType : uint32_t
{
    Nothing = 0,
    Ellipse = 1,
    RoundedRectangle = 2,
    Rectangle = 3,
};

struct FragPushConstants
{
    // color(vec4) in solid color
    // start(vec2) + end(vec2) in linear gradient
    // center(vec2) + offset(vec2) in radial gradient
    // offset(vec2) + scale(vec2) in image
    glm::vec4 color;

    uint32_t shapeType = static_cast<uint32_t>(ShapeType::Nothing);
    uint32_t patternType = static_cast<uint32_t>(PatternType::SolidColor);
    glm::vec2 padd;

    // x: alpha
    // radiusX(float) (=y) + radiusY(float) (=z) in radial gradient
    // imageSize(vec2) (=yz) + uvMode(float) (=w) in image (0 = uv(image), 1 = window coordinates(image pattern))
    glm::vec4 patternParams;

    // radius (vec2): ellipse
    // to bottom-right vector(vec2), radius(vec2): rounded rect
    // to bottom-right vector(vec2): rect(nothing)
    glm::vec4 shapeParams;
};

struct VertexPushConstants
{
    glm::mat4 model;
};

struct PushConstant
{
    VertexPushConstants v;
    FragPushConstants f;
};
}

#else
#ifdef __SLANG__

module push_constants;
public struct TransformMatrix
{
    public float4x4 model;
};

public struct FragPushConstants
{
    public float4 color;
    public uint   shapeType;
    public uint   patternType;
    public float2 padd;
    public float4 patternParams;
    public float4 shapeParams;
};

public struct PushConstants
{
    public TransformMatrix v;
    public FragPushConstants f;
};

[[vk::push_constant]]
public ConstantBuffer<PushConstants> push;

#endif // __SLANG__

#endif // __cplusplus


#endif //GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H
