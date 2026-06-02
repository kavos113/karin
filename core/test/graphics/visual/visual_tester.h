#ifndef CORE_TEST_GRAPHICS_VISUAL_VISUAL_TESTER_H
#define CORE_TEST_GRAPHICS_VISUAL_VISUAL_TESTER_H

#include <cstdlib>

#include <string>
#include <vector>
#include <iostream>

#include <stb_image/stb_image_write.h>
#include <stb_image/stb_image.h>

class VisualTester
{
public:
    static bool checkOrUpdate(
        const std::string& testName,
        const std::vector<std::byte>& actualImageData,
        int width,
        int height
    )
    {
        std::string expectedImagePath = "test_images/" + testName + ".png";

        const char* envUpdate = std::getenv("UPDATE_TEST_IMAGES");
        bool updateTestImages = envUpdate != nullptr && std::string(envUpdate) == "1";
        if (updateTestImages)
        {
            stbi_write_png(
                expectedImagePath.c_str(),
                width,
                height,
                4,
                actualImageData.data(),
                width * 4
            );
            std::cout << "[INFO] Updated test image: " << expectedImagePath << std::endl;
            return true;
        }

        int w, h, channels;
        unsigned char* expectedImageData = stbi_load(expectedImagePath.c_str(), &w, &h, &channels, 4);
        if (expectedImageData == nullptr)
        {
            std::cerr << "[ERROR] Expected image not found: " << expectedImagePath << std::endl;
            return false;
        }

        if (w != width || h != height)
        {
            std::cerr << "[ERROR] Image size mismatch for " << testName << ": expected " << w << "x" << h << ", got " << width << "x" << height << std::endl;
            stbi_image_free(expectedImageData);
            return false;
        }

        bool result = compareImages(
            reinterpret_cast<const unsigned char*>(actualImageData.data()),
            expectedImageData,
            width,
            height
        );

        if (!result)
        {
            std::cerr << "[ERROR] Image comparison failed for " << testName << std::endl;
        }

        return result;
    }

private:
    static constexpr float THRESHOLD = 0.01f;

    static bool compareImages(
        const unsigned char* actualImageData,
        const unsigned char* expectedImageData,
        int width,
        int height
    )
    {
        size_t pixelCount = width * height;
        size_t maxDiffPixels = static_cast<size_t>(THRESHOLD * pixelCount);
        size_t diffPixelCount = 0;

        for (size_t i = 0; i < pixelCount * 4; i += 4)
        {
            int diffR = std::abs(actualImageData[i + 0] - expectedImageData[i + 0]);
            int diffG = std::abs(actualImageData[i + 1] - expectedImageData[i + 1]);
            int diffB = std::abs(actualImageData[i + 2] - expectedImageData[i + 2]);
            int diffA = std::abs(actualImageData[i + 3] - expectedImageData[i + 3]);

            if ((diffR + diffG + diffB + diffA) > 10)
            {
                diffPixelCount++;
                if (diffPixelCount > maxDiffPixels)
                {
                    std::cerr << "[ERROR] Image mismatch: " << diffPixelCount << " pixels differ (threshold: " << maxDiffPixels << ")" << std::endl;
                    return false;
                }
            }
        }
        return true;
    }
};

#endif //CORE_TEST_GRAPHICS_VISUAL_VISUAL_TESTER_H
