cmake_minimum_required(VERSION 3.12)

# Change ProjectName and version to your liking.
project(ProjectName VERSION 0.1.0)

# To compile first creare a "build" directory in this folder.
# "mkdir build"

# Enter the build directory
# "cd build"

# Run cmake:
# "cmake .."

# To make a debug build:
# "cmake --build ."

# To make a release build:
# "cmake --build . --config Release"

# The generated binaries will be placed in the "build_output" folder

set(CMAKE_CXX_STANDARD 17)

# Add you source files here
set(SOURCES
    src/main.cpp
#   src/example.cpp 
)

# Add your header files here
set(HEADERS
#   src/example.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add a macro like this:
#target_compile_definitions(${PROJECT_NAME} PRIVATE SOME_MACRO)

# This line creates the source code directory structure in visual studio.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX Sources FILES ${SOURCES} ${HEADERS})


# GLAD
add_subdirectory(dependencies/glad)
target_link_libraries(${PROJECT_NAME} glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw)
target_link_libraries(${PROJECT_NAME} glfw)
