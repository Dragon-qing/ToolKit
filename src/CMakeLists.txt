if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_resources(QT_RESOURCES ${PROJECT_SOURCE_DIR}/res.qrc)
else()
    qt5_add_resources(QT_RESOURCES ${PROJECT_SOURCE_DIR}/res.qrc)
endif()

if(WIN32)
    set(APP_ICON_RESOURCE ${PROJECT_SOURCE_DIR}/app.rc)
endif()


# 子模块
add_subdirectory(core)
add_subdirectory(ui)
add_subdirectory(utils)
add_subdirectory(modules)
add_subdirectory(thirdparty)

add_executable(DQToolKits
    main.cpp
    mainwindow.cpp
    mainwindow.ui
    ${QT_RESOURCES}
    ${APP_ICON_RESOURCE}
)

target_link_libraries(DQToolKits PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport

    core_lib
    ui_lib
    utils_lib
    modules_lib
    thirdparty_lib

    user32
    qhotkey
    EasyQtSql
    spdlog
)

# Debug 不加 WIN32，Release 加 WIN32(无控制台的 GUI 程序)
set_target_properties(DQToolKits PROPERTIES
    WIN32_EXECUTABLE $<$<NOT:$<CONFIG:Debug>>:ON>
)

install(TARGETS DQToolKits RUNTIME DESTINATION bin)