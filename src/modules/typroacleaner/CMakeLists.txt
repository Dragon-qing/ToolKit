add_library(typroacleaner_lib)

target_sources(typroacleaner_lib PRIVATE
    widget/wgtyproaimgcleaner.cpp
    widget/wgtyproaimgcleaner.ui
)

target_include_directories(typroacleaner_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    widget
)

target_link_libraries(typroacleaner_lib PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    core_lib
    utils_lib
    ui_lib
)