cmake_minimum_required(VERSION 3.28)

set(EXAMPLE_NAME layer)

project(graphics_${EXAMPLE_NAME})

set(CMAKE_CXX_STANDARD 20)

add_executable(graphics_${EXAMPLE_NAME} main.cpp)
add_dependencies(graphics_${EXAMPLE_NAME}
        karin_system
        karin_graphics
)

target_link_libraries(graphics_${EXAMPLE_NAME} PRIVATE
        karin_system
        karin_graphics
)

target_include_directories(graphics_${EXAMPLE_NAME} PRIVATE ${INCLUDE_DIR})

set_target_properties(graphics_${EXAMPLE_NAME} PROPERTIES FOLDER examples/graphics)