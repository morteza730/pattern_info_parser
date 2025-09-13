set(CMAKE_AUTOMOC ON)

file(GLOB HEAD_FILES
    ${LIBPARSER_PROJECT_DIR}/include/parser/*.hpp
)

file(GLOB SRC_FILES
    ${LIBPARSER_PROJECT_DIR}/src/parser/*.hpp
    ${LIBPARSER_PROJECT_DIR}/src/parser/*.cpp
)

add_library(${PARSER_TARGET_NAME} SHARED ${HEAD_FILES})

target_sources(${PARSER_TARGET_NAME} PRIVATE
    ${SRC_FILES}
)

target_link_libraries(${PARSER_TARGET_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Xml
)

set_target_properties(${PARSER_TARGET_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/libs
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/libs
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/libs
)

target_include_directories(${PARSER_TARGET_NAME}
    PUBLIC
        ${LIBPARSER_PROJECT_DIR}/include
)

target_include_directories(${PARSER_TARGET_NAME}
    PRIVATE
        ${LIBPARSER_PROJECT_DIR}/include/parser
)
