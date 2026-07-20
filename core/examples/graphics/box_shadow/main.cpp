#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    std::unique_ptr<karin::Window> window = app.createWindow("Hello Graphics", 100, 100, 800, 600);

    karin::WindowRenderer renderer(window.get());

    renderer.setClearColor(karin::Color(karin::Color::White));

    karin::Pattern greyPattern = karin::SolidColorPattern(karin::Color(0.7f, 0.7f, 0.7f));

    renderer.addDrawCommand(
        [&greyPattern](karin::GraphicsContext& gc)
        {
            gc.fillBoxShadow(karin::Rectangle(100, 100, 300, 200), karin::Color(karin::Color::Red), 15.0f, 0.0f);
            gc.fillRect(karin::Rectangle(100, 100, 300, 200), greyPattern);
        }
    );

    renderer.update();

    window->setStatus(karin::Window::ShowStatus::SHOW);

    karin::EventPayload event;
    while (app.waitEvent(event)) {}

    renderer.cleanUp();

    return 0;
}