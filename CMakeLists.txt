cmake_minimum_required(VERSION 3.10)

project(PatternParser LANGUAGES CXX)

option(LIBPARSER_BUILD_DEMOS "Build all of libparser's own demos." ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PARSER_TARGET_NAME ${PROJECT_NAME})
set(LIBPARSER_PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5 REQUIRED COMPONENTS Core Xml)

include(GNUInstallDirs)

add_subdirectory(src)

if (LIBPARSER_BUILD_DEMOS)
    add_subdirectory(demo)
endif()
