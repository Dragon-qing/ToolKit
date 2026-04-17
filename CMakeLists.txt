cmake_minimum_required(VERSION 3.16)

project(DQToolKits LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
enable_language(RC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Keep generated files and runtime output close to the original .pro layout.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(LIBS_DIR "${CMAKE_SOURCE_DIR}/lib")

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Sql Xml Widgets PrintSupport)
set(QT_VERSION_MAJOR 5)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Sql Xml Widgets PrintSupport)
# MSVC 警告等级
add_compile_options(/W3)

qt5_add_resources(QT_RESOURCES ${CMAKE_SOURCE_DIR}/res.qrc)
set(APP_ICON_RESOURCE "")
if(WIN32)
    set(APP_ICON_RESOURCE ${CMAKE_SOURCE_DIR}/app.rc)
endif()

set(PROJECT_SOURCES
    ./src/main.cpp
    ./src/mainwindow.cpp
    # core
    ./src/core/basewidget.cpp
    ./src/core/sysapi.cpp
    ./src/core/widgetmanger.cpp
    # 数据层
    ./src/model/datacomrpt.cpp
    ./src/model/dataconfig.cpp
    ./src/model/datalog.cpp
    ./src/model/datapainter.cpp
    ./src/model/dataxmleditor.cpp
    ./src/model/msgdata.cpp
    # 服务层
    ./src/service/dataservice.cpp
    # 界面层
    ./src/widget/components/basetable.cpp
    ./src/widget/components/unionplot.cpp

    ./src/widget/dlgabout.cpp
    ./src/widget/dlgarchivemakeinfo.cpp
    ./src/widget/dlgarchiveprocess.cpp
    ./src/widget/dlgcomres.cpp
    ./src/widget/dlghelp.cpp
    ./src/widget/dlgprompt.cpp
    ./src/widget/dlgunionplotsubline.cpp
    ./src/widget/wgarchivemaker.cpp
    ./src/widget/wgcomrpt.cpp
    ./src/widget/wglog.cpp
    ./src/widget/wgrenametool.cpp
    ./src/widget/wgtyproaimgcleaner.cpp
    ./src/widget/wgxmleditor.cpp
    # 工具类
    ./src/utils/tklogger.cpp
    ./src/utils/common.cpp
    ./src/utils/externaltool/externaltoolbase.cpp
    ./src/utils/externaltool/exeexternaltool.cpp
    ./src/utils/externaltool/shellexternaltool.cpp
    ./src/utils/externaltool/apiexternaltool.cpp
    ./src/utils/externaltool/toolfactory.cpp

    # 第三方
    ./src/thirdparty/qcustomplot.cpp
)

set(PROJECT_HEADERS
    ./src/mainwindow.h
    # core
    ./src/core/basewidget.h
    ./src/core/datadef.h
    ./src/core/sysapi.h
    ./src/core/widgetmanger.h
    # 数据层
    ./src/model/datacomrpt.h
    ./src/model/dataconfig.h
    ./src/model/datalog.h
    ./src/model/datapainter.h
    ./src/model/dataxmleditor.h
    ./src/model/msgdata.h
    # 服务层
    ./src/service/dataservice.h
    # 界面层
    ./src/widget/components/basetable.h
    ./src/widget/components/unionplot.h

    ./src/widget/dlgabout.h
    ./src/widget/dlgarchivemakeinfo.h
    ./src/widget/dlgarchiveprocess.h
    ./src/widget/dlgcomres.h
    ./src/widget/dlghelp.h
    ./src/widget/dlgprompt.h
    ./src/widget/dlgunionplotsubline.h
    ./src/widget/wgarchivemaker.h
    ./src/widget/wgcomrpt.h
    ./src/widget/wglog.h
    ./src/widget/wgrenametool.h
    ./src/widget/wgtyproaimgcleaner.h
    ./src/widget/wgxmleditor.h
    # 工具类
    ./src/utils/tklogger.h
    ./src/utils/common.h
    ./src/utils/externaltool/externaltoolbase.h
    ./src/utils/externaltool/exeexternaltool.h
    ./src/utils/externaltool/shellexternaltool.h
    ./src/utils/externaltool/apiexternaltool.h
    ./src/utils/externaltool/toolfactory.h
    ./src/utils/externaltool/factoryregister.h

    # 第三方
    ./src/thirdparty/qcustomplot.h
    )

set(PROJECT_UI
    ./src/mainwindow.ui
    # core
    ./src/core/basewidget.ui
    ./src/core/widgetmanger.ui
    # 数据层
    ./src/model/datapainter.ui
    # 界面层
    ./src/widget/components/basetable.ui
    ./src/widget/components/unionplot.ui

    ./src/widget/dlgabout.ui
    ./src/widget/dlgarchivemakeinfo.ui
    ./src/widget/dlgarchiveprocess.ui
    ./src/widget/dlgcomres.ui
    ./src/widget/dlghelp.ui
    ./src/widget/dlgprompt.ui
    ./src/widget/dlgunionplotsubline.ui
    ./src/widget/wgarchivemaker.ui
    ./src/widget/wgcomrpt.ui
    ./src/widget/wglog.ui
    ./src/widget/wgrenametool.ui
    ./src/widget/wgtyproaimgcleaner.ui
    ./src/widget/wgxmleditor.ui
)


add_executable(DQToolKits
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UI}
    ${QT_RESOURCES}
    ${APP_ICON_RESOURCE}
)

# Debug 不加 WIN32，Release 加 WIN32(无控制台的 GUI 程序)
set_target_properties(DQToolKits PROPERTIES
    WIN32_EXECUTABLE $<$<NOT:$<CONFIG:Debug>>:ON>
)

target_include_directories(DQToolKits PRIVATE
    ./src
    ./src/core
    ./src/widget
    ./src/widget/components
    ./src/model
    ./src/service
    ./src/thirdparty
    ./src/utils
    ./src/utils/externaltool
    ./include
)

# 添加第三方库
add_library(EasyQtSql INTERFACE)

target_include_directories(EasyQtSql INTERFACE
    ./src/thirdparty/easyqtsql
)

add_library(qhotkey STATIC IMPORTED)
set_target_properties(qhotkey PROPERTIES
    IMPORTED_LOCATION_RELEASE "${LIBS_DIR}/qhotkey.lib"
    IMPORTED_LOCATION_DEBUG   "${LIBS_DIR}/debug/qhotkey.lib"
)

add_library(spdlog STATIC IMPORTED)
set_target_properties(spdlog PROPERTIES
    IMPORTED_LOCATION_RELEASE "${LIBS_DIR}/spdlog.lib"
    IMPORTED_LOCATION_DEBUG   "${LIBS_DIR}/debug/spdlogd.lib"
)

target_link_libraries(DQToolKits PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
    user32
    qhotkey
    EasyQtSql
    spdlog
)

if (MSVC)
    target_compile_options(DQToolKits PRIVATE /utf-8)
endif()

install(TARGETS DQToolKits RUNTIME DESTINATION bin)
