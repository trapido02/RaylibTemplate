cmake_minimum_required(VERSION 4.0)
project(RaylibTemplate LANGUAGES C CXX)

# Disallow in-source builds
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Please use a separate build directory.")
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
set(RAYLIB_VERSION "LATEST" CACHE STRING "Version of raylib to use")
option(RAYLIB_BUILD_EXAMPLES "Build raylib example programs" OFF)
option(RAYLIB_SHARED "Build raylib as shared library" OFF)

# FetchContent or find_package
if (NOT RAYLIB_VERSION STREQUAL "LATEST")
    find_package(raylib ${RAYLIB_VERSION} QUIET)
else ()
    find_package(raylib QUIET)
endif ()

if (NOT raylib_FOUND)
    include(FetchContent)

    # Set raylib directories to avoid conflicts with other versions
    set(_raylib_source_dir "${CMAKE_BINARY_DIR}/_deps/raylib-src")
    set(_raylib_binary_dir "${CMAKE_BINARY_DIR}/_deps/raylib-build")

    if (RAYLIB_VERSION STREQUAL "LATEST")
        message(STATUS "Fetching latest raylib from GitHub")
        FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG master
            SOURCE_DIR ${_raylib_source_dir}
            BINARY_DIR ${_raylib_binary_dir}
        )
    else ()
        message(STATUS "Fetching raylib version ${RAYLIB_VERSION}")
        FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
            SOURCE_DIR ${_raylib_source_dir}
            BINARY_DIR ${_raylib_binary_dir}
        )
    endif ()

    # Configure raylib build options before building
    set(BUILD_SHARED_LIBS ${RAYLIB_SHARED} CACHE BOOL "Build raylib shared" FORCE)
    set(BUILD_EXAMPLES ${RAYLIB_BUILD_EXAMPLES} CACHE BOOL "Build raylib examples" FORCE)

    FetchContent_MakeAvailable(raylib)
endif ()

# Define source files
file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp"
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

# Include dirs for your own code
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Linking
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    raylib
)

# Output directories, handle multi-config
set(output_dir "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${output_dir}"
    LIBRARY_OUTPUT_DIRECTORY "${output_dir}"
    ARCHIVE_OUTPUT_DIRECTORY "${output_dir}"
)

foreach (OUTPUT_CONFIG IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG_UPPER)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin/${OUTPUT_CONFIG}
        LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib/${OUTPUT_CONFIG}
        ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib/${OUTPUT_CONFIG}
    )
endforeach ()