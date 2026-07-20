#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, BoxShadow)
{
    karin::OffscreenRenderer renderer(800, 600);

    renderer.setClearColor(karin::Color(karin::Color::White));

    karin::Pattern greyPattern = karin::SolidColorPattern(karin::Color(0.7f, 0.7f, 0.7f));

    renderer.addDrawCommand(
        [&greyPattern](karin::GraphicsContext& gc)
        {
            gc.fillBoxShadow(karin::Rectangle(100, 100, 300, 200), karin::Color(karin::Color::Red), 15.0f, 0.0f);
            gc.fillRect(karin::Rectangle(100, 100, 300, 200), greyPattern);
        }
    );

    renderer.draw();

    bool result = VisualTester::checkOrUpdate(
        "box_shadow",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}