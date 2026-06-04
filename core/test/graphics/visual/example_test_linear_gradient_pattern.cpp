#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, LinearGradientPattern)
{
    karin::OffscreenRenderer renderer(800, 600);

    karin::Pattern pattern = karin::LinearGradientPattern{
        .start = karin::Point(0, 0),
        .end = karin::Point(500, 500),
        .gradientPoints = {
            .points = {
                {0.0f, karin::Color(karin::Color::Red)},
                {0.5f, karin::Color(karin::Color::Green)},
                {1.0f, karin::Color(karin::Color::Blue)}
            },
            .extendMode = karin::ExtendMode::CLAMP
        },
    };

    renderer.addDrawCommand(
        [&pattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(karin::Rectangle(50, 50, 600, 400), pattern);
        }
    );

    renderer.draw();

    bool result = VisualTester::checkOrUpdate(
        "linear_gradient_pattern",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}