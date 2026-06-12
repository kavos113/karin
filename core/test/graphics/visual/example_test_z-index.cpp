#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, ZIndex)
{
    karin::OffscreenRenderer renderer(800, 600);

    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Pattern cyanPattern = karin::SolidColorPattern(karin::Color(0, 255, 255));
    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
    karin::Pattern yellowPattern = karin::SolidColorPattern(karin::Color(255, 255, 0));

    renderer.addDrawCommand(
        [&cyanPattern, &magentaPattern, &yellowPattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(karin::Rectangle(100, 100, 300, 300), cyanPattern);
            gc.fillRect(karin::Rectangle(150, 150, 300, 300), magentaPattern);
            gc.translateZ(-1);
            gc.fillRect(karin::Rectangle(200, 200, 300, 300), yellowPattern);
        }
    );

    renderer.draw();

    bool result = VisualTester::checkOrUpdate(
        "z_index",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}