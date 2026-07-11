#include <karin/common.h>
#include <karin/gui.h>

#include <memory>
#include <random>

int main()
{
    karin::gui::Application app;
    auto window = app.createWindow("Opacity", 100, 100, 800, 600);

    auto rootView = std::make_unique<karin::gui::ContainerNode>(karin::Size(800, 600));

    for (int i = 0; i < 10; ++i)
    {
        auto rect = std::make_unique<karin::gui::RectangleNode>(
            karin::Size(100, 100),
            karin::Color(0.0f, 0.0f, 1.0f)
        );

        rect->setOpacity(1.0 - 0.07 * i);

        rootView->addChild(std::move(rect));
    }

    auto bigRect = std::make_unique<karin::gui::RectangleNode>(
        karin::Size(200, 200),
        karin::Color(0.0f, 1.0f, 1.0f)
    );
    for (int i = 0; i < 5; ++i)
    {
        auto rect = std::make_unique<karin::gui::RectangleNode>(
            karin::Size(50, 50),
            karin::Color(0.0f, 1.0f, 0.0f)
        );
        bigRect->addChild(std::move(rect));
    }
    bigRect->setLayoutDirection(karin::gui::ContainerNode::LayoutDirection::Column);
    bigRect->setGap(5.0f);
    bigRect->setWrapMode(karin::gui::ContainerNode::WrapMode::Wrap);
    bigRect->setPadding(karin::gui::ViewNode::Side::All, 10.0f);
    bigRect->setOpacity(0.2f);
    rootView->addChild(std::move(bigRect));

    rootView->setLayoutDirection(karin::gui::ContainerNode::LayoutDirection::Row);
    rootView->setGap(10.0f);
    rootView->setWrapMode(karin::gui::ContainerNode::WrapMode::Wrap);
    rootView->setBackgroundColor(karin::Color(0.7f, 0.7f, 0.0f));

    window->setRootView(std::move(rootView));
    app.run();
}