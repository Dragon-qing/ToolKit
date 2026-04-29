add_library(ui_lib)

target_sources(ui_lib PRIVATE
    widgetmanager.cpp
    widgetmanager.ui
    uifactory.cpp
)

target_include_directories(ui_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ui_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets 
    utils_lib
    core_lib
    qhotkey
    thirdparty_lib
    components_lib
)
