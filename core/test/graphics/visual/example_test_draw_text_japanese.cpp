#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, DrawTextJapanese)
{
    karin::OffscreenRenderer renderer(800, 600);
    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::TextEngine textEngine;
    textEngine.registerCustomFont("fixtures/font/NotoSansJP-Regular.ttf", karin::Font("Noto Sans"));

    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
    karin::TextBlob textBlob = textEngine.layoutText(
    "日本語も描画できます。とても長い文章の場合、改行やトリミングが適切に行われるか確認します。",
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
        "draw_text_japanese",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}