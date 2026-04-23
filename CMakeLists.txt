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
    ./src/core/system/sysapi.cpp
    ./src/core/model/msgdata.cpp
    ./src/core/logging/tklogger.cpp
    ./src/core/config/configsetting.cpp
    ./src/core/externaltool/externaltoolbase.cpp
    ./src/core/externaltool/exeexternaltool.cpp
    ./src/core/externaltool/shellexternaltool.cpp
    ./src/core/externaltool/apiexternaltool.cpp
    ./src/core/externaltool/toolfactory.cpp
    ./src/core/threadmanager/threadmanager.cpp
    # ui
    ./src/ui/base/basewidget.cpp
    ./src/ui/components/basetable.cpp
    ./src/ui/components/unionplot.cpp
    ./src/ui/components/dlgprompt.cpp
    ./src/ui/widgetmanager.cpp
    # 工具
    ./src/utils/common.cpp
    ./src/utils/stringopt.cpp
    ./src/utils/xmlopt.cpp
    # 第三方
    ./src/thirdparty/qcustomplot.cpp
    # modules
    # about模块
    ./src/modules/about/widget/dlgabout.cpp
    # archive模块
    ./src/modules/archive/widget/dlgarchivemakeinfo.cpp
    ./src/modules/archive/widget/dlgarchiveprocess.cpp
    ./src/modules/archive/widget/wgarchivemaker.cpp
    # comrpt模块
    ./src/modules/comrpt/model/datacomrpt.cpp
    ./src/modules/comrpt/widget/dlgcomres.cpp
    ./src/modules/comrpt/widget/dlgunionplotsubline.cpp
    ./src/modules/comrpt/widget/wgcomrpt.cpp
    # help模块
    ./src/modules/help/widget/dlghelp.cpp
    # logviewer模块
    ./src/modules/logviewer/model/datalog.cpp
    ./src/modules/logviewer/widget/wglog.cpp
    # renametool模块
    ./src/modules/renametool/widget/wgrenametool.cpp
    ./src/modules/renametool/service/renametoolservice.cpp
    # typroacleaner模块
    ./src/modules/typroacleaner/widget/wgtyproaimgcleaner.cpp
    # xmleditor模块
    ./src/modules/xmleditor/model/dataxmleditor.cpp
    ./src/modules/xmleditor/widget/wgxmleditor.cpp
    # thememanager模块
    ./src/modules/thememanager/model/datapainter.cpp
)

set(PROJECT_HEADERS
    ./src/mainwindow.h
    # core
    ./src/core/system/sysapi.h
    ./src/core/model/msgdata.h
    ./src/core/model/datadef.h
    ./src/core/config/configsetting.h
    ./src/core/logging/tklogger.h
    ./src/core/externaltool/externaltoolbase.h
    ./src/core/externaltool/exeexternaltool.h
    ./src/core/externaltool/shellexternaltool.h
    ./src/core/externaltool/apiexternaltool.h
    ./src/core/externaltool/toolfactory.h
    ./src/core/externaltool/factoryregister.h
    ./src/core/threadmanager/threadmanager.h
    # ui
    ./src/ui/base/basewidget.h
    ./src/ui/components/basetable.h
    ./src/ui/components/unionplot.h
    ./src/ui/components/dlgprompt.h
    ./src/ui/widgetmanager.h
    # 工具
    ./src/utils/common.h
    ./src/utils/stringopt.h
    ./src/utils/xmlopt.h
    # 第三方
    ./src/thirdparty/qcustomplot.h
    # modules
    # about模块
    ./src/modules/about/widget/dlgabout.h
    # archive模块
    ./src/modules/archive/widget/dlgarchivemakeinfo.h
    ./src/modules/archive/widget/dlgarchiveprocess.h
    ./src/modules/archive/widget/wgarchivemaker.h
    # comrpt模块
    ./src/modules/comrpt/model/datacomrpt.h
    ./src/modules/comrpt/widget/dlgcomres.h
    ./src/modules/comrpt/widget/dlgunionplotsubline.h
    ./src/modules/comrpt/widget/wgcomrpt.h
    # help模块
    ./src/modules/help/widget/dlghelp.h
    # logviewer模块
    ./src/modules/logviewer/widget/wglog.h
    ./src/modules/logviewer/model/datalog.h
    # renametool模块
    ./src/modules/renametool/widget/wgrenametool.h
    ./src/modules/renametool/model/renametooldata.h
    ./src/modules/renametool/service/renametoolservice.h
    # typroacleaner模块
    ./src/modules/typroacleaner/widget/wgtyproaimgcleaner.h
    # xmleditor模块
    ./src/modules/xmleditor/model/dataxmleditor.h
    ./src/modules/xmleditor/widget/wgxmleditor.h
    # thememanager模块
    ./src/modules/thememanager/model/datapainter.h
)

set(PROJECT_UI

    ./src/mainwindow.ui
    # ui
    ./src/ui/base/basewidget.ui
    ./src/ui/components/basetable.ui
    ./src/ui/components/unionplot.ui
    ./src/ui/components/dlgprompt.ui
    ./src/ui/widgetmanager.ui
    # modules
    # about模块
    ./src/modules/about/widget/dlgabout.ui
    # archive模块
    ./src/modules/archive/widget/dlgarchivemakeinfo.ui
    ./src/modules/archive/widget/dlgarchiveprocess.ui
    ./src/modules/archive/widget/wgarchivemaker.ui
    # comrpt模块
    ./src/modules/comrpt/widget/dlgcomres.ui
    ./src/modules/comrpt/widget/dlgunionplotsubline.ui
    ./src/modules/comrpt/widget/wgcomrpt.ui
    # help模块
    ./src/modules/help/widget/dlghelp.ui
    # logviewer模块
    ./src/modules/logviewer/widget/wglog.ui
    # renametool模块
    ./src/modules/renametool/widget/wgrenametool.ui
    # thememanager模块
    ./src/modules/thememanager/model/datapainter.ui
    # typroacleaner模块
    ./src/modules/typroacleaner/widget/wgtyproaimgcleaner.ui
    # xmleditor模块
    ./src/modules/xmleditor/widget/wgxmleditor.ui
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
    ./src/core/model
    ./src/core/logging
    ./src/core/config
    ./src/core/externaltool
    ./src/core/system
    ./src/core/threadmanager
    ./src/ui
    ./src/ui/base
    ./src/ui/components
    ./src/utils
    ./src/thirdparty
    ./src/thirdparty/EasyQtSql
    ./include
    # about模块
    ./src/modules/about/widget
    # archive模块
    ./src/modules/archive/widget
    # comrpt模块
    ./src/modules/comrpt/widget
    ./src/modules/comrpt/model
    # help模块
    ./src/modules/help/widget
    # logviewer模块
    ./src/modules/logviewer/widget
    ./src/modules/logviewer/model
    # renametool模块
    ./src/modules/renametool/widget
    ./src/modules/renametool/model
    ./src/modules/renametool/service
    # thememanager模块
    ./src/modules/thememanager/model
    # typroacleaner模块
    ./src/modules/typroacleaner/widget
    # xmleditor模块
    ./src/modules/xmleditor/model
    ./src/modules/xmleditor/widget
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
