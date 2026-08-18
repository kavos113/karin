#include <karin/system.h>

#include <thread>
#include <chrono>
#include <string>

int main()
{
    karin::Application& app = karin::Application::instance();
    std::unique_ptr<karin::Window> window = app.createWindow("Hello Window", 100, 100, 800, 600);

    window->setStatus(karin::Window::ShowStatus::SHOW);

    window->addResizeCallback([](karin::Size newSize)
    {
        std::cout << "Window resized to " << newSize.width << "x" << newSize.height << std::endl;
    });

    std::thread timer([&window]
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        window->sendActionEvent(10, std::string("from timer"));
    });

    karin::EventPayload event;
    while (app.waitEvent(event))
    {
        if (const auto* windowEvent = std::get_if<karin::WindowEvent>(&event.event))
        {
            if (windowEvent->type == karin::WindowEvent::Type::Close)
            {
                std::cout << "Window close event received." << std::endl;
            }
        }
        else if (const auto* actionEvent = std::get_if<karin::ActionEvent>(&event.event))
        {
            std::cout << "event has_value: " << actionEvent->data.has_value() << std::endl;
            std::cout << "event data type: " << actionEvent->data.type().name() << std::endl;
            std::cout << "get action event id = " << actionEvent->actionId << ", message = " << std::any_cast<std::string>(actionEvent->data) << std::endl;
        }
    }

    timer.join();

    return 0;
}