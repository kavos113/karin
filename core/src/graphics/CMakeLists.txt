set(GRAPHICS_SOURCE
        renderer_base.cpp
        window_renderer.cpp
        offscreen_renderer.cpp
        graphics_context.cpp
        path.cpp
        path_impl.cpp
        hash.cpp
        canvas.cpp
        ${THIRD_PARTY_DIR}/stb_image/stb_image_impl.cpp
        ${COMMON_DIR}/geometry/transform2d.cpp

        text/text_layouter.cpp
        text/text_engine.cpp
        text/text_engine_impl.cpp
        text/font_loader.cpp
        text/font_manager.cpp
)
set(GRAPHICS_HEADERS
        font_renderer_impl.h
        offscreen_renderer_target.h
        path_impl.h
        platform.h
        renderer_impl.h
        painter.h
        canvas.h
        draw_instruction.h
        ${THIRD_PARTY_DIR}/stb_image/stb_image.h

        text/font_loader.h
        text/font_loader_platform.h
        text/font_manager.h
        text/harfbuzz_provider.h
        text/platform.h
        text/text_engine_impl.h
        text/text_layouter.h
)

if (DIRECTX)
    set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
            d2d/d2d_window_surface.cpp
            d2d/d2d_offscreen_surface.cpp
            d2d/d2d_renderer_impl.cpp
            d2d/d2d_offscreen_renderer_impl.cpp
            d2d/d2d_painter.cpp
            d2d/d2d_device_resources.cpp
            d2d/d2d_context.cpp
            d2d/d2d_font_renderer.cpp
            text/windows/dwrite_font_face.cpp
            text/windows/dwrite_font_loader.cpp
    )
    set(GRAPHICS_HEADERS ${GRAPHICS_HEADERS}
            d2d/d2d_window_surface.h
            d2d/d2d_offscreen_surface.h
            d2d/d2d_surface.h
            d2d/d2d_renderer_impl.h
            d2d/d2d_offscreen_renderer_impl.h
            d2d/d2d_painter.h
            d2d/d2d_device_resources.h
            d2d/d2d_context.h
            d2d/d2d_font_renderer.h
            d2d/d2d_geometry.h
            d2d/d2d_consts.h
            d2d/d2d_color.h
            text/windows/dwrite_font_face.h
            text/windows/dwrite_font_loader.h
            text/windows/dwrite_converter.h
    )

elseif (VULKAN)
    set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
            vulkan/vulkan_context.cpp
            vulkan/vulkan_debug_manager.cpp
            vulkan/vulkan_window_surface.cpp
            vulkan/vulkan_offscreen_surface.cpp
            vulkan/vulkan_renderer_impl.cpp
            vulkan/vulkan_painter.cpp
            vulkan/vulkan_pipeline.cpp
            vulkan/vulkan_tessellator.cpp
            vulkan/vulkan_device_resources.cpp
            vulkan/vulkan_glyph_cache.cpp
            vulkan/vulkan_font_renderer.cpp
            vulkan/vulkan_view_context.cpp
            vulkan/vulkan_geometry_buffer.cpp
            vulkan/vulkan_frame_context.cpp
            vulkan/vulkan_offscreen_frame_context.cpp
            vulkan/vma_impl.cpp
            text/freetype/freetype_font_face.cpp
    )
    set(GRAPHICS_HEADERS ${GRAPHICS_HEADERS}
            vulkan/vulkan_context.h
            vulkan/vulkan_debug_manager.h
            vulkan/vulkan_window_surface.h
            vulkan/vulkan_offscreen_surface.h
            vulkan/vulkan_surface.h
            vulkan/vulkan_renderer_impl.h
            vulkan/vulkan_painter.h
            vulkan/vulkan_pipeline.h
            vulkan/vulkan_tessellator.h
            vulkan/vulkan_device_resources.h
            vulkan/vulkan_glyph_cache.h
            vulkan/vulkan_font_renderer.h
            vulkan/vulakn_functions.h
            vulkan/vulkan_buffer.h
            vulkan/vulkan_surface.h
            vulkan/vulkan_helpers.h
            vulkan/vulkan_view_context.h
            vulkan/vulkan_geometry_buffer.h
            vulkan/vulkan_frame_context.h
            vulkan/vulkan_offscreen_frame_context.h
            vulkan/glm_geometry.h
            vulkan/vma.h
            vulkan/shaders/shader_code.h
            vulkan/shaders/shader_layout.h
            vulkan/shaders/push_constants.slang
            text/freetype/freetype_font_face.h
    )

    if (WIN32)
        set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
                text/windows/freetype_dwrite_font_loader.cpp
                d2d/d2d_context.cpp
        )
        set(GRAPHICS_HEADERS ${GRAPHICS_HEADERS}
                text/windows/freetype_dwrite_font_loader.h
                text/windows/dwrite_converter.h
                d2d/d2d_context.h
        )
    elseif (LINUX)
        set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
                text/unix/freetype_fontconfig_font_loader.cpp
        )
        set(GRAPHICS_HEADERS ${GRAPHICS_HEADERS}
                text/unix/freetype_fontconfig_font_loader.h
        )
    endif ()
endif ()

# ------------ link libraries ------------

add_library(karin_graphics STATIC ${GRAPHICS_SOURCE} ${GRAPHICS_HEADERS})
target_include_directories(karin_graphics PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_graphics PRIVATE ${COMMON_DIR})
target_include_directories(karin_graphics PRIVATE ${THIRD_PARTY_DIR})
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(glm CONFIG REQUIRED)
target_link_libraries(karin_graphics PRIVATE glm::glm)
if (DIRECTX)
    target_link_libraries(karin_graphics PRIVATE d2d1.lib)
    target_link_libraries(karin_graphics PRIVATE dxgi.lib)
    target_link_libraries(karin_graphics PRIVATE d3d11.lib)
    target_link_libraries(karin_graphics PRIVATE dwrite.lib)

    find_package(harfbuzz CONFIG REQUIRED)
    target_link_libraries(karin_graphics PRIVATE harfbuzz::harfbuzz)

    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_DIRECTX)
    target_compile_definitions(karin_graphics PUBLIC UNICODE)
    target_compile_definitions(karin_graphics PUBLIC NOMINMAX)
    target_compile_definitions(karin_graphics PUBLIC WIN32_LEAN_AND_MEAN)
elseif (VULKAN)
    set(SHADER_CODE_OUT_FILE vulkan/shaders/shader_code.h)
    set(SHADER_LAYOUT_OUT_FILE vulkan/shaders/shader_layout.h)

    add_custom_command(
            OUTPUT ${SHADER_CODE_OUT_FILE} ${SHADER_LAYOUT_OUT_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND slang_shader_compiler
                    ${SHADER_CODE_OUT_FILE} ${SHADER_LAYOUT_OUT_FILE} ${SOURCE_DIR}
                    vulkan/shaders/vertex.slang
                    vulkan/shaders/geometry.slang
                    vulkan/shaders/text.slang
                    vulkan/shaders/shadow.slang
            DEPENDS
                    slang_shader_compiler
                    vulkan/shaders/vertex.slang
                    vulkan/shaders/geometry.slang
                    vulkan/shaders/text.slang
                    vulkan/shaders/push_constants.slang
                    vulkan/shaders/shadow.slang
            COMMENT "Compiling graphics shaders"
    )
    add_custom_target(
            shaders_graphics ALL
            DEPENDS ${SHADER_CODE_OUT_FILE} ${SHADER_LAYOUT_OUT_FILE}
    )

    add_dependencies(karin_graphics shaders_graphics)

    if (WIN32)
        target_link_libraries(karin_graphics PRIVATE d2d1.lib)
        target_link_libraries(karin_graphics PRIVATE dxgi.lib)
        target_link_libraries(karin_graphics PRIVATE d3d11.lib)
        target_link_libraries(karin_graphics PRIVATE dwrite.lib)
    elseif (LINUX)
        find_package(Fontconfig REQUIRED)
        target_link_libraries(karin_graphics PRIVATE Fontconfig::Fontconfig)
    endif ()

    find_package(Vulkan REQUIRED)
    target_link_libraries(karin_graphics PRIVATE Vulkan::Vulkan)

    find_package(VulkanMemoryAllocator CONFIG REQUIRED)
    target_link_libraries(karin_graphics PRIVATE GPUOpen::VulkanMemoryAllocator)

    find_package(Freetype CONFIG REQUIRED)
    target_link_libraries(karin_graphics PRIVATE Freetype::Freetype)

    find_package(harfbuzz CONFIG REQUIRED)
    target_link_libraries(karin_graphics PRIVATE harfbuzz::harfbuzz)

    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_VULKAN)
endif ()

if (WIN32)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_WINDOWS)
elseif (APPLE)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_MACOS)
elseif (UNIX)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_UNIX)
endif ()

if (HEADLESS)
    target_compile_definitions(karin_graphics PUBLIC KARIN_OPTION_HEADLESS)
endif ()