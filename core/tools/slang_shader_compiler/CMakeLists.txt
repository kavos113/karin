cmake_minimum_required(VERSION 3.28)
project(slang_shader_compiler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(slang_shader_compiler
        main.cpp
        print.cpp
        print.h
        slang_context.h
        shader_module.h
        util.h
        generator.cpp
        generator.h
)

find_package(Vulkan REQUIRED)
find_package(slang REQUIRED)
target_link_libraries(slang_shader_compiler PRIVATE
        Vulkan::Vulkan
        slang::slang
)
