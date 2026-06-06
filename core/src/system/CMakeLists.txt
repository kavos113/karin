set(SYSTEM_SOURCE
        application.cpp
        window.cpp
)
set(SYSTEM_HEADER
        application_impl.h
        window_impl.h
        platform.h
)

if (WIN32)
    set(SYSTEM_SOURCE ${SYSTEM_SOURCE}
            windows/win_application_impl.cpp
            windows/win_window_impl.cpp
            windows/win_window_class_registry.cpp
            windows/win_event.cpp
            windows/win_converter.cpp
            windows/win_context.cpp
    )
    set(SYSTEM_HEADER ${SYSTEM_HEADER}
            windows/win_application_impl.h
            windows/win_window_impl.h
            windows/win_window_class_registry.h
            windows/win_event.h
            windows/win_converter.h
            windows/win_context.h

    )
elseif (UNIX)
    set(SYSTEM_SOURCE ${SYSTEM_SOURCE}
            x11/x11_application_impl.cpp
            x11/x11_window_impl.cpp
            x11/x11_converter.cpp
            x11/x11_context.cpp
            x11/x11_event.cpp
    )
    set(SYSTEM_HEADER ${SYSTEM_HEADER}
            x11/x11_application_impl.h
            x11/x11_window_impl.h
            x11/x11_converter.h
            x11/x11_context.h
            x11/x11_event.h
    )
endif ()

add_library(karin_system STATIC ${SYSTEM_SOURCE} ${SYSTEM_HEADER})
target_include_directories(karin_system PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_system PRIVATE ${COMMON_DIR})
target_include_directories(karin_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
    target_compile_definitions(karin_system PUBLIC KARIN_PLATFORM_WINDOWS)
    target_compile_definitions(karin_system PUBLIC UNICODE)
    target_compile_definitions(karin_system PUBLIC NOMINMAX)
    target_compile_definitions(karin_system PUBLIC WIN32_LEAN_AND_MEAN)
elseif (APPLE)
    target_compile_definitions(karin_system PUBLIC KARIN_PLATFORM_MACOS)
elseif (UNIX)
    target_compile_definitions(karin_system PUBLIC KARIN_PLATFORM_UNIX)
endif ()

if (UNIX)
    target_link_libraries(karin_system PRIVATE X11)
endif ()