#------------------------------------------------------------------------------
# Executable production
#------------------------------------------------------------------------------
set(RAYTRACER_SOURCES "")
set(RAYTRACER_HEADERS "")

file (GLOB_RECURSE RAYTRACER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file (GLOB_RECURSE RAYTRACER_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.[.h|.hpp]")

foreach (_headerFile ${RAYTRACER_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND RAYTRACER_INCLUDE_DIRS ${_dir})
endforeach()

list (REMOVE_DUPLICATES RAYTRACER_SOURCES)
list (REMOVE_DUPLICATES RAYTRACER_HEADERS)
list (REMOVE_DUPLICATES RAYTRACER_LIBRARY)
list (REMOVE_DUPLICATES RAYTRACER_INCLUDE_DIRS)

# Create executable
add_executable(${PROJECT_NAME} ${RAYTRACER_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${RAYTRACER_INCLUDE_DIRS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYTRACER_LIBRARY})
