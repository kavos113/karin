set(GUI_SOURCES
        view_node.cpp
        window.cpp
        container_node.cpp
        rectangle_node.cpp
        leaf_node.cpp
        application.cpp
        application_context.cpp
        text_node.cpp
        window_event_dispatcher.cpp
        application_event_dispatcher.cpp
        event.cpp
        timer.cpp
)
set(GUI_HEADERS
        application_context.h
        window_event_dispatcher.h
        application_event_dispatcher.h
)

add_library(karin_gui STATIC ${GUI_SOURCES} ${GUI_HEADERS})
target_include_directories(karin_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(karin_gui PRIVATE ${COMMON_DIR})
target_include_directories(karin_gui PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_gui PRIVATE ${THIRD_PARTY_DIR})

find_package(yoga CONFIG REQUIRED)
target_link_libraries(karin_gui PRIVATE yoga::yogacore)