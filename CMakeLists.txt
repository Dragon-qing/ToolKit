cmake_minimum_required(VERSION 3.16)

project(DQToolKits LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
enable_language(RC)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(Options)
include(Dependencies)

add_subdirectory(src)

install(TARGETS DQToolKits RUNTIME DESTINATION bin)
