add_library(modules_lib INTERFACE)

add_subdirectory(about)
add_subdirectory(archive)
add_subdirectory(comrpt)
add_subdirectory(logviewer)
add_subdirectory(renametool)
add_subdirectory(typroacleaner)
add_subdirectory(xmleditor)
add_subdirectory(thememanager)

target_link_libraries(modules_lib INTERFACE
    about_lib
    archive_lib
    comrpt_lib
    logviewer_lib
    renametool_lib
    typroacleaner_lib
    xmleditor_lib
    thememanager_lib
)