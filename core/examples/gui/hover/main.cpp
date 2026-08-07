#include <karin/common.h>
#include <karin/gui.h>

#include <memory>
#include <random>

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    karin::gui::Application app;
    auto window = app.createWindow("Hello Layout", 100, 100, 800, 600);

    auto rootView = std::make_unique<karin::gui::ContainerNode>(karin::Size(800, 600));

    karin::Color hoveredColor = karin::Color(dis(gen), dis(gen), dis(gen));
    karin::Color color = karin::Color(dis(gen), dis(gen), dis(gen));
    auto rect = std::make_unique<karin::gui::RectangleNode>(
        karin::Size(100, 100),
        color
    );
    rect->setPointerHandler(
        karin::gui::ViewNode::EventType::PointerEnter,
        [&hoveredColor, &rect](karin::Point point, karin::MouseButtonType type, int delta)
        {
            rect->setColor(hoveredColor);
            rect->requestRedraw();
        }
    );
    rect->setPointerHandler(
        karin::gui::ViewNode::EventType::PointerLeave,
        [&rect, &color](karin::Point point, karin::MouseButtonType type, int delta)
        {
            rect->setColor(color);
            rect->requestRedraw();
        }
    );
    rootView->addChild(rect.get());

    rootView->setLayoutDirection(karin::gui::ContainerNode::LayoutDirection::Row);
    rootView->setGap(10.0f);
    rootView->setWrapMode(karin::gui::ContainerNode::WrapMode::Wrap);

    window->setRootView(std::move(rootView));
    app.run();
}