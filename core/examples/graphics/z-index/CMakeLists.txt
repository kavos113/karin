cmake_minimum_required(VERSION 3.28)
project(karin_examples_z-index)

set(CMAKE_CXX_STANDARD 20)

add_executable(karin_examples_z-index main.cpp)
add_dependencies(karin_examples_z-index karin_system karin_graphics)
target_link_libraries(karin_examples_z-index PRIVATE karin_system karin_graphics)
target_include_directories(karin_examples_z-index PRIVATE ${INCLUDE_DIR})