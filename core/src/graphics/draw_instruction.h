#ifndef CORE_SRC_GRAPHICS_DRAW_INSTRUCTION_H
#define CORE_SRC_GRAPHICS_DRAW_INSTRUCTION_H

#include <variant>

#include <karin/common/geometry/rectangle.h>
#include <karin/common/geometry/point.h>
#include <karin/graphics/path.h>
#include <karin/graphics/stroke_style.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/image.h>
#include <karin/graphics/graphics_context.h>
#include <karin/graphics/text_blob.h>

namespace karin
{

struct DrawInstructionFillRect;
struct DrawInstructionFillEllipse;
struct DrawInstructionFillRoundedRect;
struct DrawInstructionFillPath;
struct DrawInstructionDrawLine;
struct DrawInstructionDrawRect;
struct DrawInstructionDrawEllipse;
struct DrawInstructionDrawRoundedRect;
struct DrawInstructionDrawPath;
struct DrawInstructionDrawImage;
struct DrawInstructionDrawText;

using DrawInstruction = std::variant<
    DrawInstructionFillRect,
    DrawInstructionFillEllipse,
    DrawInstructionFillRoundedRect,
    DrawInstructionFillPath,
    DrawInstructionDrawLine,
    DrawInstructionDrawRect,
    DrawInstructionDrawEllipse,
    DrawInstructionDrawRoundedRect,
    DrawInstructionDrawPath,
    DrawInstructionDrawImage,
    DrawInstructionDrawText
>;

struct DrawInstructionFillRect
{
    Rectangle rect;
    Pattern pattern;
    GraphicsContext::State state;
};

struct DrawInstructionFillEllipse
{
    Point center;
    float radiusX;
    float radiusY;
    Pattern pattern;
    GraphicsContext::State state;
};

struct DrawInstructionFillRoundedRect
{
    Rectangle rect;
    float radiusX;
    float radiusY;
    Pattern pattern;
    GraphicsContext::State state;
};

struct DrawInstructionFillPath
{
    Path path;
    Pattern pattern;
    GraphicsContext::State state;
};

struct DrawInstructionDrawLine
{
    Point start;
    Point end;
    Pattern pattern;
    StrokeStyle strokeStyle;
    GraphicsContext::State state;
};

struct DrawInstructionDrawRect
{
    Rectangle rect;
    Pattern pattern;
    StrokeStyle strokeStyle;
    GraphicsContext::State state;
};

struct DrawInstructionDrawEllipse
{
    Point center;
    float radiusX;
    float radiusY;
    Pattern pattern;
    StrokeStyle strokeStyle;
    GraphicsContext::State state;
};

struct DrawInstructionDrawRoundedRect
{
    Rectangle rect;
    float radiusX;
    float radiusY;
    Pattern pattern;
    StrokeStyle strokeStyle;
    GraphicsContext::State state;
};

struct DrawInstructionDrawPath
{
    Path path;
    Pattern pattern;
    StrokeStyle strokeStyle;
    GraphicsContext::State state;
};

struct DrawInstructionDrawImage
{
    Image image;
    Rectangle destRect;
    Rectangle srcRect;
    float opacity;
    GraphicsContext::State state;
};

struct DrawInstructionDrawText
{
    TextBlob text;
    Point start;
    Pattern pattern;
    GraphicsContext::State state;
};

}

#endif //CORE_SRC_GRAPHICS_DRAW_INSTRUCTION_H
