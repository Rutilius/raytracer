cmake_minimum_required(VERSION 3.14)
project(Raytracer   VERSION 0.1.0
                    DESCRIPTION "Ray Tracer made following the Peter Shirley: Ray Tracing in one week book series"
                    LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)
    add_compile_options(-W -Wall)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

set(BIN_PATH "../../.package")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_PATH}/release/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_PATH}/release/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH}/release/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BIN_PATH}/debug/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BIN_PATH}/debug/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_PATH}/debug/bin)

set(RAYTRACER_LIBRARY "")
set(RAYTRACER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

# add_subdirectory(cmake)
add_subdirectory(include)
add_subdirectory(src)

