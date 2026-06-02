#include <karin/graphics.h>
#include <karin/common.h>

#include <stb_image/stb_image_write.h>
#include <vector>

int main()
{
    karin::OffscreenRenderer renderer(800, 600);

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

    int result = stbi_write_png(
        "offscreen_render.png",
        800,
            600,
        4,
        imageData.data(),
        800 * 4
    );
    if (result == 0)
    {
        printf("Failed to write image\n");
    }
    else
    {
        printf("Image written successfully\n");
    }

    renderer.cleanUp();

    return 0;
}