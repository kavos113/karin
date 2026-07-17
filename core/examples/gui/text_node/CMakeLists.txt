cmake_minimum_required(VERSION 3.28)

set(EXAMPLE_NAME text_node)

project(gui_${EXAMPLE_NAME})

set(CMAKE_CXX_STANDARD 20)

add_executable(gui_${EXAMPLE_NAME} main.cpp)
add_dependencies(gui_${EXAMPLE_NAME}
        karin_system
        karin_graphics
        karin_gui
)

target_link_libraries(gui_${EXAMPLE_NAME} PRIVATE
        karin_system
        karin_graphics
        karin_gui
)

target_include_directories(gui_${EXAMPLE_NAME} PRIVATE ${INCLUDE_DIR})

set_target_properties(gui_${EXAMPLE_NAME} PROPERTIES FOLDER examples/gui)

find_package(yoga CONFIG REQUIRED)
target_link_libraries(gui_${EXAMPLE_NAME} PRIVATE yoga::yogacore)