cmake_minimum_required(VERSION 3.16)

project(toolkits LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Keep generated files and runtime output close to the original .pro layout.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
set(LIBS_DIR "${CMAKE_SOURCE_DIR}/../libs")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Sql Xml Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Sql Xml Widgets PrintSupport)

set(PROJECT_SOURCES
    ../src/basewidget.cpp
    ../src/common.cpp
    ../src/hmi/hmicomrpt.cpp
    ../src/hmi/hmilog.cpp
    ../src/hmi/sysapi.cpp
    ../src/logdt.cpp
    ../src/main.cpp
    ../src/mainwindow.cpp
    ../src/msgdata.cpp
    ../src/service/dataservice.cpp
    ../src/hmi/hmiconfig.cpp
    ../src/hmi/hmipainter.cpp
    ../src/hmi/hmixmleditor.cpp
    ../src/widget/control/basetable.cpp
    ../src/widget/dlgabout.cpp
    ../src/widget/dlgbtfmakeinfo.cpp
    ../src/widget/dlgbtfprocess.cpp
    ../src/widget/dlgcomres.cpp
    ../src/widget/dlghelp.cpp
    ../src/widget/dlgprompt.cpp
    ../src/widget/dlgunionplotsubline.cpp
    ../src/widget/unionplot.cpp
    ../src/widget/wgbtfmake.cpp
    ../src/widget/wgcomrpt.cpp
    ../src/widget/wglog.cpp
    ../src/widget/wgrenametool.cpp
    ../src/widget/wgtyproaimgcleaner.cpp
    ../src/widget/wgxmleditor.cpp
    ../src/widget/widgetmanger.cpp
    ../src/thirdparty/qcustomplot.cpp
)

set(PROJECT_HEADERS
    ../src/basewidget.h
    ../src/common.h
    ../src/datadef.h
    ../src/hmi/hmicomrpt.h
    ../src/hmi/hmilog.h
    ../src/hmi/sysapi.h
    ../src/logdt.h
    ../src/mainwindow.h
    ../src/msgdata.h
    ../src/service/dataservice.h
    ../src/hmi/hmiconfig.h
    ../src/hmi/hmipainter.h
    ../src/hmi/hmixmleditor.h
    ../src/widget/control/basetable.h
    ../src/widget/dlgabout.h
    ../src/widget/dlgbtfmakeinfo.h
    ../src/widget/dlgbtfprocess.h
    ../src/widget/dlgcomres.h
    ../src/widget/dlghelp.h
    ../src/widget/dlgprompt.h
    ../src/widget/dlgunionplotsubline.h
    ../src/widget/unionplot.h
    ../src/widget/wgbtfmake.h
    ../src/widget/wgcomrpt.h
    ../src/widget/wglog.h
    ../src/widget/wgrenametool.h
    ../src/widget/wgtyproaimgcleaner.h
    ../src/widget/wgxmleditor.h
    ../src/widget/widgetmanger.h
    ../src/thirdparty/qcustomplot.h
)

set(PROJECT_UI
    ../src/basewidget.ui
    ../src/mainwindow.ui
    ../src/hmi/hmipainter.ui
    ../src/widget/control/basetable.ui
    ../src/widget/dlgabout.ui
    ../src/widget/dlgbtfmakeinfo.ui
    ../src/widget/dlgbtfprocess.ui
    ../src/widget/dlgcomres.ui
    ../src/widget/dlghelp.ui
    ../src/widget/dlgprompt.ui
    ../src/widget/dlgunionplotsubline.ui
    ../src/widget/unionplot.ui
    ../src/widget/wgbtfmake.ui
    ../src/widget/wgcomrpt.ui
    ../src/widget/wglog.ui
    ../src/widget/wgrenametool.ui
    ../src/widget/wgtyproaimgcleaner.ui
    ../src/widget/wgxmleditor.ui
    ../src/widget/widgetmanger.ui
)

add_executable(toolkits WIN32
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UI}
    ../res.qrc
)

target_include_directories(toolkits PRIVATE
    ../src
    ../src/widget
    ../src/hmi
    ../src/service
    ../src/thirdparty
    ../src/thirdparty/easyqtsql
    ../src/widget/control
    ../include
)

# 添加第三方库
add_library(EasyQtSql INTERFACE)

target_include_directories(EasyQtSql INTERFACE
    ../src/thirdparty/easyqtsql
)

add_library(qhotkey STATIC IMPORTED)
set_target_properties(qhotkey PROPERTIES
    IMPORTED_LOCATION "${LIBS_DIR}/qhotkey.lib"
)

target_link_libraries(toolkits PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
    user32
    qhotkey
    EasyQtSql
)

if (MSVC)
    target_compile_options(toolkits PRIVATE /utf-8)
endif()

install(TARGETS toolkits RUNTIME DESTINATION bin)
