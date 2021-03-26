cmake_minimum_required(VERSION 3.12)

project(BlockCraft VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)


set(SOURCES
	src/main.cpp
	src/Application.cpp
	src/Application.hpp

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
	
	src/Utils/NoCopy.hpp
	src/Utils/ScopeGuard.cpp
	src/Utils/ScopeGuard.hpp
	src/Utils/SetOperations.hpp
	src/Utils/Version.hpp
	
	src/Vk/Instance.cpp
	src/Vk/Instance.hpp
	
	src/Vulkan/PhysicalDeviceID.cpp
	src/Vulkan/PhysicalDeviceID.hpp
	src/Vulkan/VulkanInstance.cpp
	src/Vulkan/VulkanInstance.hpp
	src/Vulkan/VulkanSettings.hpp
	src/Vulkan/VulkanWindow.cpp
	src/Vulkan/VulkanWindow.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add a macro:
#target_compile_definitions(${PROJECT_NAME} PRIVATE SOME_MACRO)


# Create the source code directory structure in visual studio.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX Source FILES ${SOURCES} ${HEADERS})

# Set as default startup item.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)

message(WARNING ${Vulkan_FOUND})
message(WARNING ${Vulkan_INCLUDE_DIRS})
message(WARNING ${Vulkan_LIBRARIES})


target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Vulkan_LIBRARIES})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw)

#target_include_directories(glfw PUBLIC ${VULKAN_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)


#GLM
target_include_directories(${PROJECT_NAME} PRIVATE dependencies/glm)
