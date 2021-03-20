cmake_minimum_required(VERSION 3.12)

project(BlockCraft VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
	src/main.cpp

	src/Game/BlockID.hpp
	src/Game/BlockType.cpp
	src/Game/BlockType.hpp
	src/Game/Chunk.cpp
	src/Game/Chunk.hpp
	src/Game/World.cpp
	src/Game/World.hpp

	src/Game/Blocks/GrassBlock.cpp
	src/Game/Blocks/GrassBlock.hpp
	src/Game/Blocks/SandBlock.cpp
	src/Game/Blocks/SandBlock.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add a macro:
#target_compile_definitions(${PROJECT_NAME} PRIVATE SOME_MACRO)

# This line creates the source code directory structure in visual studio.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX Sources FILES ${SOURCES} ${HEADERS})


# GLAD
add_subdirectory(dependencies/glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw)

target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} glfw)
