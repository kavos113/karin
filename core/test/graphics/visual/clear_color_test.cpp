#include <gtest/gtest.h>

#include <vector>

#include <karin/graphics/offscreen_renderer.h>
#include <karin/common/color/color.h>
#include "visual_tester.h"

using namespace karin;

TEST(ClearColorTest, ClearToRed)
{
    OffscreenRenderer renderer(10, 10);
    renderer.setClearColor(Color(255, 0, 0));

    renderer.draw();

    std::vector<std::byte> imageData = renderer.getImageData();

    ASSERT_EQ(imageData.size(), 10 * 10 * 4); // 10x10 pixels, BGRA format
    for (size_t i = 0; i < imageData.size(); i += 4)
    {
        uint8_t blue = std::to_integer<uint8_t>(imageData[i]);
        uint8_t green = std::to_integer<uint8_t>(imageData[i + 1]);
        uint8_t red = std::to_integer<uint8_t>(imageData[i + 2]);
        uint8_t alpha = std::to_integer<uint8_t>(imageData[i + 3]);

        EXPECT_EQ(red, 255);
        EXPECT_EQ(green, 0);
        EXPECT_EQ(blue, 0);
        EXPECT_EQ(alpha, 255);
    }
}