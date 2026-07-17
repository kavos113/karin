cmake_minimum_required(VERSION 3.28)

set(EXAMPLE_NAME set_size)

project(system_${EXAMPLE_NAME})

set(CMAKE_CXX_STANDARD 20)

add_executable(system_${EXAMPLE_NAME} main.cpp)
add_dependencies(system_${EXAMPLE_NAME} karin_system)

target_link_libraries(system_${EXAMPLE_NAME} PRIVATE karin_system)
target_include_directories(system_${EXAMPLE_NAME} PRIVATE ${INCLUDE_DIR})

set_target_properties(system_${EXAMPLE_NAME} PROPERTIES FOLDER examples/system)