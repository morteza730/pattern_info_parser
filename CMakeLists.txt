cmake_minimum_required(VERSION 3.10)

project(PatternParser LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_TARGET_NAME cparser)
set(APPLICATION_TARGET_NAME prog)

find_package(Qt5 REQUIRED COMPONENTS Core Xml)

include(GNUInstallDirs)

add_subdirectory(cparser)
add_subdirectory(app)
