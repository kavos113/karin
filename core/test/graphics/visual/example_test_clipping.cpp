#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, Clipping)
{
    karin::OffscreenRenderer renderer(800, 600);

    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Pattern redPattern = karin::SolidColorPattern(karin::Color(karin::Color::Red));
    karin::Pattern bluePattern = karin::SolidColorPattern(karin::Color(karin::Color::Blue));

    renderer.addDrawCommand(
        [&redPattern, &bluePattern](karin::GraphicsContext& gc)
        {
            gc.withSave([&]
            {
                gc.clip(karin::Rectangle(150, 100, 300, 300));

                gc.fillRect(karin::Rectangle(100, 100, 200, 200), redPattern);
                gc.fillEllipse(karin::Point(400, 300), 100, 50, bluePattern);
                gc.fillRoundedRect(karin::Rectangle(500, 100, 200, 200), 20, 50, redPattern);
            });
        }
    );

    renderer.draw();

    bool result = VisualTester::checkOrUpdate(
        "clipping",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}