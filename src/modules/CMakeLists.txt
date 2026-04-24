add_library(modules_lib)

add_subdirectory(about)
add_subdirectory(archive)
add_subdirectory(comrpt)
add_subdirectory(help)
add_subdirectory(logviewer)
add_subdirectory(renametool)
add_subdirectory(typroacleaner)
add_subdirectory(xmleditor)
add_subdirectory(thememanager)

target_link_libraries(modules_lib PUBLIC
    about_lib
    archive_lib
    comrpt_lib
    help_lib
    logviewer_lib
    renametool_lib
    typroacleaner_lib
    xmleditor_lib
    thememanager_lib
)