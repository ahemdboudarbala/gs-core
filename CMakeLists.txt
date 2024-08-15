cmake_minimum_required(VERSION 3.15)

# Set the project name and version
project(GraphStreamCpp VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable position-independent code (useful for shared libraries)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define build options
option(BUILD_SHARED_LIBS "Build as a shared library" ON)
option(ENABLE_TESTS "Build unit tests" ON)

# Define the output directories for binaries and libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include custom modules (optional, if you have any)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Set the source and include directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Automatically collect all source files
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

# Automatically collect all header files (for IDEs and include path)
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.hpp")

# Add the main library target
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Specify the include directories for the library
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

# Add compiler options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Set additional target properties (e.g., versioning, SOVERSION)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME ${PROJECT_NAME}
)

# Add installation rules (optional)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${INCLUDE_DIR}/ DESTINATION include)

# Optionally enable testing
#if(ENABLE_TESTS)
#    enable_testing()
#    add_subdirectory(src-tests)
#endif()

# Generate export header for the library (optional)
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# Optionally install the export header
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_export.h DESTINATION include)

# Export the library for use with other CMake projects
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)
