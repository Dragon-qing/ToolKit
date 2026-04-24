add_library(help_lib)

target_sources(help_lib PRIVATE
    widget/dlghelp.cpp
    widget/dlghelp.ui
)

target_include_directories(help_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    widget
)

target_link_libraries(help_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets
    ui_lib
)