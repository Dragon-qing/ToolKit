add_library(archive_lib)

target_sources(archive_lib PRIVATE
    widget/dlgarchivemakeinfo.cpp
    widget/dlgarchivemakeinfo.ui
    widget/dlgarchiveprocess.cpp
    widget/dlgarchiveprocess.ui
    widget/wgarchivemaker.cpp
    widget/wgarchivemaker.ui
    service/archiveservice.cpp
)

target_include_directories(archive_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    widget
    service
)

target_link_libraries(archive_lib PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    core_lib
    utils_lib
    components_lib
)