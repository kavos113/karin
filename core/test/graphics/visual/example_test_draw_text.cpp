#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, DrawText)
{
    karin::OffscreenRenderer renderer(800, 600);
    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::TextEngine textEngine;
    textEngine.registerCustomFont("fixtures/font/NotoSans-Regular.ttf", karin::Font("Noto Sans"));

    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
    karin::TextBlob textBlob = textEngine.layoutText(
        "Hello karin graphics!\nThis is a text layout example.\nthis is very verrrrrrrrrrrrrrrrrrrrrrrrrrry long line to test trimming and wrapping.",
        karin::TextStyle(24.0f, "Noto Sans"),
        karin::ParagraphStyle{
            .lineSpacing = 1.5f,
            .baseline = 1.0f,
            .lineSpacingMode = karin::ParagraphStyle::LineSpacingMode::PROPORTIONAL,
            .trimming = karin::ParagraphStyle::Trimming::WORD,
            .wrapping = karin::ParagraphStyle::Wrapping::WORD,
        },
        {400.0f, 200.0f}
    );

    renderer.addDrawCommand(
        [&magentaPattern, &textBlob](karin::GraphicsContext& gc)
        {
            gc.drawRect(karin::Rectangle(50.0f, 50.0f, 400.0f, 200.0f), magentaPattern);
            gc.drawText(textBlob, karin::Point(50.0f, 50.0f), magentaPattern);
        }
    );

    renderer.draw();

    bool result = VisualTester::checkOrUpdate(
        "hello_graphics",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}