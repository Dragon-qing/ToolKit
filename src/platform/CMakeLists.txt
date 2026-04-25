add_library(platform_lib)

target_sources(platform_lib PRIVATE
    sysapi.cpp
)

target_include_directories(platform_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(platform_lib PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    
    common_lib
)
