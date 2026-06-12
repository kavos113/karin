#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    std::unique_ptr<karin::Window> window = app.createWindow("Hello Graphics", 100, 100, 800, 600);

    karin::WindowRenderer renderer(window.get());

    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Pattern cyanPattern = karin::SolidColorPattern(karin::Color(0, 255, 255));
    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
    karin::Pattern yellowPattern = karin::SolidColorPattern(karin::Color(255, 255, 0));

    renderer.addDrawCommand(
        [&cyanPattern, &magentaPattern, &yellowPattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(karin::Rectangle(100, 100, 300, 300), cyanPattern);
            gc.fillRect(karin::Rectangle(150, 150, 300, 300), magentaPattern);
            gc.translateZ(-1);
            gc.fillRect(karin::Rectangle(200, 200, 300, 300), yellowPattern);
        }
    );

    renderer.update();

    window->setStatus(karin::Window::ShowStatus::SHOW);

    karin::EventPayload event;
    while (app.waitEvent(event)) {}

    renderer.cleanUp();

    return 0;
}