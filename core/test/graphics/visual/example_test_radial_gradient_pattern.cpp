#include <gtest/gtest.h>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, RadialGradientPattern)
{
    karin::OffscreenRenderer renderer(800, 600);

    karin::Pattern pattern = karin::RadialGradientPattern{
        .center = karin::Point(400, 300),
        .offset = karin::Point(100, 100),
        .radiusX = 300.0f,
        .radiusY = 200.0f,
        .gradientPoints = {
            .points = {
                {0.0f, karin::Color(karin::Color::Red)},
                {0.5f, karin::Color(karin::Color::Green)},
                {1.0f, karin::Color(karin::Color::Blue)}
            },
            .extendMode = karin::ExtendMode::REPEAT
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
        "radial_gradient_pattern",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}