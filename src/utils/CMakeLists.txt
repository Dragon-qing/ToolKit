add_library(utils_lib)

target_sources(utils_lib PRIVATE
    common.cpp
    stringopt.cpp
    xmlopt.cpp
)

target_include_directories(utils_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(utils_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml

    common_lib
    platform_lib
)