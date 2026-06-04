#include <karin/graphics.h>
#include <karin/common.h>

#include <stb_image/stb_image_write.h>
#include <vector>
#include <iostream>

int main()
{
    const int width = 800;
    const int height = 600;

    karin::OffscreenRenderer renderer(width, height);

    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Pattern cyanPattern = karin::SolidColorPattern(karin::Color(0, 255, 255));
    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));

    renderer.addDrawCommand(
        [&cyanPattern, &magentaPattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(karin::Rectangle(100, 100, 200, 200), cyanPattern);
            gc.fillRect(karin::Rectangle(400, 100, 200, 200), magentaPattern);
        }
    );

    renderer.draw();

    std::vector<std::byte> imageData = renderer.getImageData();
    if (imageData.empty() || imageData.size() != width * height * 4)
    {
        std::cerr << "Failed to retrieve image data or data size is incorrect" << std::endl;
        return -1;
    }

    int result = stbi_write_png(
        "offscreen_render.png",
        width,
        height,
        4,
        imageData.data(),
        width * 4
    );
    if (result == 0)
    {
        std::cerr << "Failed to write image to file" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Image successfully saved to offscreen_render.png" << std::endl;
    }

    renderer.cleanUp();

    return 0;
}