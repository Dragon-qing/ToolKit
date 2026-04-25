add_library(renametool_lib)

target_sources(renametool_lib PRIVATE
    service/renametoolservice.cpp
    widget/wgrenametool.cpp
    widget/wgrenametool.ui
)

target_include_directories(renametool_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    model
    service
    widget
)

target_link_libraries(renametool_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets
    core_lib
    utils_lib
    components_lib
)