add_library(thirdparty_lib)

target_sources(thirdparty_lib PRIVATE
    qcustomplot.cpp
    qhotkey.cpp
    easyqtsql/easyqtsql_clangd.cpp
    spdlog.cpp
)

# 改原文件仅为桥接第三方库的头文件，通常不需要 CMake 的自动处理（如 MOC、UIC 等）避免重复元对象代码。
set_source_files_properties(qhotkey.cpp PROPERTIES
    SKIP_AUTOMOC ON
)

target_include_directories(thirdparty_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(thirdparty_lib PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)