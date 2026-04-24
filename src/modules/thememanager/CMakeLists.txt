add_library(thememanager_lib)

target_sources(thememanager_lib PRIVATE
    model/datapainter.cpp
    model/datapainter.ui
)

target_include_directories(thememanager_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    model
)

target_link_libraries(thememanager_lib PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)