find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Sql Xml PrintSupport Svg)

add_library(EasyQtSql INTERFACE)
target_include_directories(EasyQtSql INTERFACE
    ${CMAKE_SOURCE_DIR}/src/thirdparty/easyqtsql
)

target_link_libraries(EasyQtSql INTERFACE
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Core
)

add_library(qhotkey STATIC IMPORTED)
set_target_properties(qhotkey PROPERTIES
    IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/lib/qhotkey.lib"
    IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/lib/debug/qhotkey.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include"
)

add_library(spdlog STATIC IMPORTED)
set_target_properties(spdlog PROPERTIES
    IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/lib/spdlog.lib"
    IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/lib/debug/spdlogd.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include"
)