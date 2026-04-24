set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(QT_VERSION_MAJOR 5)

if(WIN32)
    # Unicode
    add_definitions(-DUNICODE -D_UNICODE)

    set(APP_ICON_RESOURCE ${CMAKE_SOURCE_DIR}/app.rc)
    if (MSVC)
        add_compile_options(
            /utf-8
            /W3
        )
    endif()
endif()