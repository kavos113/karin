#include <gtest/gtest.h>

#include <cstdint>

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

    ASSERT_EQ(imageData.size(), 10 * 10 * 4); // 10x10 pixels, RGBA format
    for (size_t i = 0; i < imageData.size(); i += 4)
    {
        uint8_t r = std::to_integer<uint8_t>(imageData[i + 0]);
        uint8_t g = std::to_integer<uint8_t>(imageData[i + 1]);
        uint8_t b = std::to_integer<uint8_t>(imageData[i + 2]);
        uint8_t a = std::to_integer<uint8_t>(imageData[i + 3]);

        ASSERT_EQ(r, 255);
        ASSERT_EQ(g, 0);
        ASSERT_EQ(b, 0);
        ASSERT_EQ(a, 255);
    }
}