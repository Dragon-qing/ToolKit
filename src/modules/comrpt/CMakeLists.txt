add_library(comrpt_lib)

target_sources(comrpt_lib PRIVATE
    model/datacomrpt.cpp
    widget/dlgcomres.cpp
    widget/dlgcomres.ui
    widget/dlgunionplotsubline.cpp
    widget/dlgunionplotsubline.ui
    widget/wgcomrpt.cpp
    widget/wgcomrpt.ui
)

target_include_directories(comrpt_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    model
    widget
)

target_link_libraries(comrpt_lib PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    utils_lib
    core_lib
    ui_lib
)