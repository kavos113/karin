#ifndef GRAPHICS_DRAW_INSTRUCTION_H
#define GRAPHICS_DRAW_INSTRUCTION_H

#include <variant>
#include <vector>

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

struct DrawInstruction;

struct DrawInstructionFillRect
{
    Rectangle rect;
    Pattern pattern;
};

struct DrawInstructionFillEllipse
{
    Point center;
    float radiusX;
    float radiusY;
    Pattern pattern;
};

struct DrawInstructionFillRoundedRect
{
    Rectangle rect;
    float radiusX;
    float radiusY;
    Pattern pattern;
};

struct DrawInstructionFillPath
{
    Path path;
    Pattern pattern;
};

struct DrawInstructionFillBoxShadow
{
    Rectangle rect;
    Color color;
    float blurRadius;
    float spreadRadius;
};

struct DrawInstructionDrawLine
{
    Point start;
    Point end;
    Pattern pattern;
    StrokeStyle strokeStyle;
};

struct DrawInstructionDrawRect
{
    Rectangle rect;
    Pattern pattern;
    StrokeStyle strokeStyle;
};

struct DrawInstructionDrawEllipse
{
    Point center;
    float radiusX;
    float radiusY;
    Pattern pattern;
    StrokeStyle strokeStyle;
};

struct DrawInstructionDrawRoundedRect
{
    Rectangle rect;
    float radiusX;
    float radiusY;
    Pattern pattern;
    StrokeStyle strokeStyle;
};

struct DrawInstructionDrawPath
{
    Path path;
    Pattern pattern;
    StrokeStyle strokeStyle;
};

struct DrawInstructionDrawImage
{
    Image image;
    Rectangle destRect;
    Rectangle srcRect;
    float opacity;
};

struct DrawInstructionDrawText
{
    TextBlob text;
    Point start;
    Pattern pattern;
};

struct DrawInstructionDrawLayer
{
    Rectangle bounds;
    float alpha;
    std::vector<DrawInstruction> instructions;
};

struct DrawInstruction
{
    GraphicsContext::State state;
    std::variant<
        DrawInstructionFillRect,
        DrawInstructionFillEllipse,
        DrawInstructionFillRoundedRect,
        DrawInstructionFillPath,
        DrawInstructionFillBoxShadow,
        DrawInstructionDrawLine,
        DrawInstructionDrawRect,
        DrawInstructionDrawEllipse,
        DrawInstructionDrawRoundedRect,
        DrawInstructionDrawPath,
        DrawInstructionDrawImage,
        DrawInstructionDrawText,
        DrawInstructionDrawLayer
    > instruction;
};

}

#endif //GRAPHICS_DRAW_INSTRUCTION_H
