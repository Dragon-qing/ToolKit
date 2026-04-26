add_library(components_lib)

target_sources(components_lib PRIVATE
    basewidget.cpp
    basewidget.ui

    basetable.cpp
    basetable.ui

    dlgprompt.cpp
    dlgprompt.ui

    flowlayout.cpp

    overlay.cpp
)

target_include_directories(components_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(components_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
)

target_link_libraries(components_lib PUBLIC
    core_lib
    utils_lib
)