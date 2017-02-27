cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

project(OPENGL_EXPERIMENTS)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

message("${CMAKE_BINARY_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Various location variables
set(EXTERNAL_DEPENDS "${PROJECT_SOURCE_DIR}/external")
set(HELPERS "${PROJECT_SOURCE_DIR}/helpers")

message("${EXTERNAL_DEPENDS} ${HELPERS} ${PROJECT_SOURCE_DIR}")

# Find needed libraries
find_package(SDL2)
find_package(OpenGL)
find_package(GLEW)
find_package(GLM)

add_subdirectory(experiments/camera)
#add_subdirectory(experiments/lighting)
