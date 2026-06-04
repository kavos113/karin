#include <gtest/gtest.h>

#include <memory>
#include <numbers>

#include <karin/graphics.h>
#include <karin/common.h>

#include "visual_tester.h"

// TODO: フォントをファイルから指定できるようになったらやる
// TEST(ExamplesVisualTest, Transform)
// {
//     karin::OffscreenRenderer renderer(800, 600);
//     renderer.setClearColor(karin::Color(karin::Color::Green));
//
//     karin::TextEngine textEngine;
//     karin::TextBlob blob = textEngine.layoutText(
//         "Hello, Karin!",
//         karin::TextStyle(24, "Segoe UI"),
//         karin::ParagraphStyle(),
//         karin::Size(200, 50)
//     );
//
//     karin::Pattern cyanPattern = karin::SolidColorPattern(karin::Color(0, 255, 255));
//     karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
//     karin::Pattern yellowPattern = karin::SolidColorPattern(karin::Color(255, 255, 0));
//     karin::Pattern blackPattern = karin::SolidColorPattern(karin::Color(0, 0, 0));
//     karin::Pattern whitePattern = karin::SolidColorPattern(karin::Color(255, 255, 255));
//
//     renderer.addDrawCommand(
//         [&cyanPattern, &magentaPattern, &yellowPattern, &blackPattern, &whitePattern, &blob](karin::GraphicsContext& gc)
//         {
//             gc.fillRect(
//                 karin::Rectangle(100, 100, 100, 100),
//                 cyanPattern,
//                 karin::Transform2D()
//                     .rotate(-std::numbers::pi / 4)
//                     .translate(0, 100)
//             );
//
//             gc.fillEllipse(
//                 karin::Point(400, 300), 50, 100,
//                 magentaPattern,
//                 karin::Transform2D()
//                     .rotate(std::numbers::pi / 4)
//                     .translate(100, 0)
//             );
//
//             gc.drawLine(
//                 karin::Point(600, 100), karin::Point(700, 200),
//                 blackPattern,
//                 karin::StrokeStyle(5),
//                 karin::Transform2D()
//                     .rotate(std::numbers::pi / 6)
//                     .translate(-50, 0)
//             );
//
//             gc.drawRect(
//                 karin::Rectangle(500, 400, 150, 100),
//                 whitePattern,
//                 karin::StrokeStyle(3),
//                 karin::Transform2D()
//                     .rotate(-std::numbers::pi / 3)
//                     .translate(0, -50)
//             );
//
//             gc.drawRoundedRect(
//                 karin::Rectangle(200, 400, 150, 100),
//                 20, 20,
//                 yellowPattern,
//                 karin::StrokeStyle(4),
//                 karin::Transform2D()
//                     .rotate(std::numbers::pi / 8)
//                     .translate(0, -50)
//             );
//
//             gc.drawText(
//                 blob,
//                 karin::Point(300, 50),
//                 blackPattern,
//                 karin::Transform2D()
//                     .rotate(-std::numbers::pi / 12)
//                     .translate(0, 50)
//                     .scale(2.0f, 1.0f)
//             );
//             gc.drawRect(
//                 karin::Rectangle(300, 50, 200, 50),
//                 magentaPattern,
//                 karin::StrokeStyle(1),
//                 karin::Transform2D()
//                     .rotate(-std::numbers::pi / 12)
//                     .translate(0, 50)
//                     .scale(2.0f, 1.0f)
//             );
//         }
//     );
//
//     renderer.draw();
//
//     bool result = VisualTester::checkOrUpdate(
//         "transform",
//         renderer.getImageData(),
//         800,
//         600
//     );
//     ASSERT_TRUE(result);
//
//     renderer.cleanUp();
// }