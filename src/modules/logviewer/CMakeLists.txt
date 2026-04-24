add_library(logviewer_lib)

target_sources(logviewer_lib PRIVATE
    model/datalog.cpp
    widget/wglog.cpp
    widget/wglog.ui
)

target_include_directories(logviewer_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/thirdparty/easyqtsql
    model
    widget
)

target_link_libraries(logviewer_lib PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    core_lib
    utils_lib
    ui_lib
    EasyQtSql
)