cmake_minimum_required(VERSION 3.28)
project(karin_examples_gui_clipping)

set(CMAKE_CXX_STANDARD 20)

add_executable(karin_examples_gui_clipping main.cpp)
add_dependencies(karin_examples_gui_clipping
        karin_gui
        karin_system
        karin_graphics)
target_link_libraries(karin_examples_gui_clipping PRIVATE
        karin_gui
        karin_system
        karin_graphics)
target_include_directories(karin_examples_gui_clipping PRIVATE ${INCLUDE_DIR})

find_package(yoga CONFIG REQUIRED)
target_link_libraries(karin_examples_gui_clipping PRIVATE yoga::yogacore)