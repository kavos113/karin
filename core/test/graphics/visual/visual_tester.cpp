#include "visual_tester.h"

#include <cstdlib>

#include <filesystem>
#include <format>

#include <stb_image/stb_image_write.h>
#include <stb_image/stb_image.h>

namespace
{
void convertBGRAtoRGBA(std::vector<std::byte>& imageData)
{
    for (size_t i = 0; i < imageData.size(); i += 4)
    {
        std::byte tempR = imageData[i + 2];
        std::byte tempB = imageData[i + 0];
        imageData[i + 0] = tempR; // R
        imageData[i + 1] = imageData[i + 1]; // G
        imageData[i + 2] = tempB; // B
        // Alpha remains unchanged
    }
}
}

std::string VisualTester::expectedImageDirectory = "fixtures/test_expect_images";
std::once_flag VisualTester::directoryInitFlag;

bool VisualTester::checkOrUpdate(
    const std::string& testName,
    const std::vector<std::byte>& actualImageData,
    int width,
    int height
)
{
    std::vector<std::byte> convertedImageData = actualImageData;
    convertBGRAtoRGBA(convertedImageData);

    ensureExpectedImageDirectoryExists();
    std::string expectedImagePath = std::format("{}/{}.png", expectedImageDirectory, testName);

    std::string envUpdate = getEnv("UPDATE_TEST_IMAGES");
    if (envUpdate == "1")
    {
        int result = stbi_write_png(
            expectedImagePath.c_str(),
            width,
            height,
            4,
            convertedImageData.data(),
            width * 4
        );
        if (result == 0)
        {
            std::cerr << "[ERROR] Failed to write expected image: " << expectedImagePath << std::endl;
            return false;
        }
        else
        {
            std::cout << "[INFO] Updated expected image: " << expectedImagePath << std::endl;
        }
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
        reinterpret_cast<const unsigned char*>(convertedImageData.data()),
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

bool VisualTester::compareImages(
    const unsigned char* actualImageData,
    const unsigned char* expectedImageData,
    int width,
    int height
)
{
    size_t pixelCount = width * height;
    size_t maxDiffPixels = THRESHOLD * pixelCount;
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

void VisualTester::ensureExpectedImageDirectoryExists()
{
    std::call_once(directoryInitFlag, []
    {
        std::string profile = getEnv("TEST_PROFILE", "default");
        expectedImageDirectory = "fixtures/test_expect_images/" + profile;
    });

    if (!std::filesystem::exists(expectedImageDirectory))
    {
        std::filesystem::create_directories(expectedImageDirectory);
    }
}

std::string VisualTester::getEnv(const char* varName, const char* defaultValue)
{
#ifdef _WIN32
    char* value = nullptr;
    if (_dupenv_s(&value, nullptr, varName) != 0 || value == nullptr)
    {
        return defaultValue;
    }
    std::string result(value);
    free(value);
    return result;
#else
    const char* value = secure_getenv(varName);
    return value != nullptr ? std::string(value) : std::string(defaultValue);
#endif
    return defaultValue;
}
