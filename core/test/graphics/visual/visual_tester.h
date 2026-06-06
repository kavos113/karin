#ifndef CORE_TEST_GRAPHICS_VISUAL_VISUAL_TESTER_H
#define CORE_TEST_GRAPHICS_VISUAL_VISUAL_TESTER_H

#include <cstdlib>
#include <cstddef>

#include <string>
#include <vector>
#include <iostream>
#include <mutex>

class VisualTester
{
public:
    static bool checkOrUpdate(
        const std::string& testName,
        const std::vector<std::byte>& actualImageData,
        int width,
        int height
    );

private:
    static bool compareImages(
        const unsigned char* actualImageData,
        const unsigned char* expectedImageData,
        int width,
        int height
    );

    static void ensureExpectedImageDirectoryExists();
    static void saveDiffImage(
        const std::string& testName,
        const unsigned char* actualImageData,
        const unsigned char *expectedImageData,
        int width,
        int height
    );

    static std::string getEnv(const char* varName, const char* defaultValue = nullptr);

    static constexpr float THRESHOLD = 0.01f;

    static std::string expectedImageDirectory;
    static std::once_flag directoryInitFlag;
};

#endif //CORE_TEST_GRAPHICS_VISUAL_VISUAL_TESTER_H
