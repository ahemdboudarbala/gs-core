cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Set the project name and version
project(gs_core VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the local CMake modules folder to the CMake search path.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Enable position-independent code (useful for shared libraries)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Boost 1.85 REQUIRED COMPONENTS system unit_test_framework)
find_package(OpenSSL REQUIRED)


# Called before any other target is defined.
enable_testing()

# On Windows, we define a preprocessor symbol with the OS version to prevent
# warnings from the Boost.Asio header files.
if(WIN32)
    include(GetWinVer REQUIRED)
    get_win_ver(WINDOWS_VERSION)
endif()


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
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
set(Test_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src-test)

# Automatically collect all source files
file(GLOB_RECURSE SOURCES
        "${SOURCE_DIR}/*.cpp"
        "${SOURCE_DIR}/**/*.cpp"
        "${SOURCE_DIR}/**/**/*.cpp"

)

# Automatically collect all header files (for IDEs and include path)
file(GLOB_RECURSE HEADERS
        "${INCLUDE_DIR}/*.hpp"
        "${INCLUDE_DIR}/**/*.hpp"
        "${INCLUDE_DIR}/**/**/*.hpp"
)

file(GLOB_RECURSE Test_SOURCES "${Test_SOURCE_DIR}/**/*.cpp")

# Add the main library target
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME}
        PUBLIC
        cxx_std_17
)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${HEADERS}
)
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
)

# Tests
add_executable(${PROJECT_NAME}_tests ${Test_SOURCE_DIR})
target_compile_features(${PROJECT_NAME}_tests
        PRIVATE
        cxx_std_14
)

target_compile_definitions(${PROJECT_NAME}_tests
        PRIVATE
        $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=${WINDOWS_VERSION}>
)
target_link_libraries(${PROJECT_NAME}_tests
        PRIVATE
        ${PROJECT_NAME}
        Boost::system
)

add_test(
        NAME ${PROJECT_NAME}_tests
        COMMAND $<TARGET_FILE:${PROJECT_NAME}_tests>
)


# This tells CMake to check for a specific output to verify the test outcome.
# When all unit tests pass, Boost.Test prints "No errors detected".
set_tests_properties(${PROJECT_NAME}_tests PROPERTIES
        PASS_REGULAR_EXPRESSION ".*No errors detected"
)

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
