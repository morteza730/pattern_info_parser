set(CMAKE_AUTOMOC ON)

add_library(${LIBRARY_TARGET_NAME} SHARED
    ${CMAKE_CURRENT_LIST_DIR}/include/pattern.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/iInterpreter.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/parserFactory.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/iParser.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/iStructInfo.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/nestedData.hpp
    # ${CMAKE_CURRENT_LIST_DIR}/include/requestInfo.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/structInfo.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/dataExtractor.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/iContainer.hpp

)

target_sources(${LIBRARY_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/parser.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/simpleJSONUnpack.hpp
    #${CMAKE_CURRENT_LIST_DIR}/include/simpleXMLUnpack.hpp
)

target_sources(${LIBRARY_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/parserFactory.cpp
    #${CMAKE_CURRENT_LIST_DIR}/src/simpleXMLUnpack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/simpleJSONUnpack.cpp
)

target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Xml
)

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
