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

    auto rect = std::make_unique<karin::gui::RectangleNode>(
        karin::Size(100, 100),
        karin::Color(dis(gen), dis(gen), dis(gen))
    );
    rect->setBorder(karin::gui::ViewNode::Side::All, 5.0f, karin::Color(dis(gen), dis(gen), dis(gen)), karin::gui::NodeBorder::LineStyle::Solid);
    rootView->addChild(std::move(rect));

    auto rect2 = std::make_unique<karin::gui::RectangleNode>(
        karin::Size(100, 100),
        karin::Color(dis(gen), dis(gen), dis(gen))
    );
    rect2->setBorder(karin::gui::ViewNode::Side::All, 5.0f, karin::Color(dis(gen), dis(gen), dis(gen)), karin::gui::NodeBorder::LineStyle::Solid);
    rootView->addChild(std::move(rect2));

    rootView->setLayoutDirection(karin::gui::ContainerNode::LayoutDirection::Row);
    rootView->setGap(10.0f);
    rootView->setWrapMode(karin::gui::ContainerNode::WrapMode::Wrap);

    window->setRootView(std::move(rootView));
    app.run();
}