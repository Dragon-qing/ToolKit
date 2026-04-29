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

    help/widget/dlghelp.cpp
    help/widget/dlghelp.ui
    help/service/helpservice.cpp
)

target_include_directories(components_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    help/widget
    help/service
    help/model
)

target_link_libraries(components_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    core_lib
    utils_lib
)
