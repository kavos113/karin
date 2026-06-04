#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, HelloGraphics)
{
    karin::OffscreenRenderer renderer(800, 600);

    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Pattern cyanPattern = karin::SolidColorPattern(karin::Color(0, 255, 255));
    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));

    renderer.addDrawCommand(
        [&cyanPattern, &magentaPattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(karin::Rectangle(100, 100, 200, 200), cyanPattern);
            gc.fillRect(karin::Rectangle(400, 100, 200, 200), magentaPattern);
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