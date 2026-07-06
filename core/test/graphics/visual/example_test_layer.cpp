#include <gtest/gtest.h>

#include <numbers>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

TEST(ExamplesVisualTest, Layer)
{
    karin::OffscreenRenderer renderer(800, 600);

    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Pattern cyanPattern = karin::SolidColorPattern(karin::Color(0, 255, 255));
    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
    karin::Pattern whitePattern = karin::SolidColorPattern(karin::Color(255, 255, 255));

    renderer.addDrawCommand(
        [&cyanPattern, &magentaPattern, &whitePattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(karin::Rectangle(550, 100, 200, 200), whitePattern);
            gc.fillRect(karin::Rectangle(575, 175, 150, 50), cyanPattern);
            gc.fillEllipse(karin::Point(650, 200), 50, 50, magentaPattern);

            gc.withSave(
                [&gc, &whitePattern, &cyanPattern, &magentaPattern]
                {
                    gc.multiplyAlpha(0.7f);
                    gc.fillRect(karin::Rectangle(300, 100, 200, 200), whitePattern);
                    gc.fillRect(karin::Rectangle(325, 175, 150, 50), cyanPattern);
                    gc.fillEllipse(karin::Point(400, 200), 50, 50, magentaPattern);
                }
            );

            gc.multiplyTransform(karin::Transform2D().rotate(std::numbers::pi / 6));
            gc.withLayer(karin::Rectangle(50, 100, 200, 200), 0.7f,
                [&gc, &whitePattern, &cyanPattern, &magentaPattern]
                {
                    gc.fillRect(karin::Rectangle(50, 100, 200, 200), whitePattern);
                    gc.fillRect(karin::Rectangle(75, 175, 150, 50), cyanPattern);
                    gc.fillEllipse(karin::Point(150, 200), 50, 50, magentaPattern);
                }
            );
        }
    );

    renderer.draw();

    bool result = VisualTester::checkOrUpdate(
        "layer",
        renderer.getImageData(),
        800,
        600
    );
    ASSERT_TRUE(result);

    renderer.cleanUp();
}