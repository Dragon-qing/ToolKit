add_library(help_lib)

target_sources(help_lib PRIVATE
    widget/dlghelp.cpp
    widget/dlghelp.ui
    service/helpservice.cpp
)

target_include_directories(help_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    widget
    service
    model
)

target_link_libraries(help_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets
    components_lib
)