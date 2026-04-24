add_library(thirdparty_lib)

target_sources(thirdparty_lib PRIVATE
    qcustomplot.cpp
)

target_include_directories(thirdparty_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(thirdparty_lib PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)