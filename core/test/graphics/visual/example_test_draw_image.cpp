#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, DrawImage)
{
    karin::OffscreenRenderer renderer(800, 600);
    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Image image = renderer.createImage("fixtures/images/square.png");

    renderer.addDrawCommand(
        [&image](karin::GraphicsContext& gc)
        {
            gc.drawImage(image, karin::Rectangle(100, 100, 200, 200), karin::Rectangle(100, 100, 100, 100));
        }
    );

    renderer.draw();

    bool result = VisualTester::checkOrUpdate(
        "draw_image",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}