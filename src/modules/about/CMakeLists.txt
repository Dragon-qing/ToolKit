add_library(about_lib)

target_sources(about_lib PRIVATE
    widget/dlgabout.cpp
    widget/dlgabout.ui
)

target_include_directories(about_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    widget
)

target_link_libraries(about_lib PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    core_lib
    utils_lib
)