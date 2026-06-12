#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, Opacity)
{
    karin::OffscreenRenderer renderer(800, 600);

    renderer.setClearColor(karin::Color(karin::Color::White));

    karin::Pattern cyanPattern = karin::SolidColorPattern(karin::Color(0, 255, 255));
    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));

    renderer.addDrawCommand(
        [&cyanPattern, &magentaPattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(karin::Rectangle(100, 100, 200, 200), cyanPattern);
            gc.multiplyAlpha(0.5f);
            gc.fillRect(karin::Rectangle(200, 200, 200, 200), magentaPattern);

            gc.setAlpha(1.0f);
            gc.fillRect(karin::Rectangle(450, 100, 100, 100), magentaPattern);
        }
    );

    renderer.draw();

    bool result = VisualTester::checkOrUpdate(
        "opacity",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}