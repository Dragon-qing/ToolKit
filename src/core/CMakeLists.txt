add_library(core_lib)

target_sources(core_lib PRIVATE
    system/sysapi.cpp
    model/msgdata.cpp
    logging/tklogger.cpp
    config/configsetting.cpp
    externaltool/externaltoolbase.cpp
    externaltool/exeexternaltool.cpp
    externaltool/shellexternaltool.cpp
    externaltool/apiexternaltool.cpp
    externaltool/toolfactory.cpp
    threadmanager/threadmanager.cpp
)

target_include_directories(core_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    model
    logging
    config
    externaltool
    threadmanager
    system
)

target_link_libraries(core_lib PUBLIC 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Gui 
    Qt${QT_VERSION_MAJOR}::Xml

    utils_lib
    
    spdlog
)