add_library(xmleditor_lib)

target_sources(xmleditor_lib PRIVATE
    model/dataxmleditor.cpp
    widget/wgxmleditor.cpp
    widget/wgxmleditor.ui
)

target_include_directories(xmleditor_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    model
    widget
)

target_link_libraries(xmleditor_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets
    core_lib
    components_lib
)