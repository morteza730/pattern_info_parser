set(EXAMPLE_TARGET_NAME prog)

add_executable(${EXAMPLE_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(${EXAMPLE_TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${EXAMPLE_TARGET_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Xml
    ${PARSER_TARGET_NAME}
)

set_target_properties(${EXAMPLE_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
