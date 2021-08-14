cmake_minimum_required(VERSION 3.12)

project(BlockCraft VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)


set(SOURCES
	src/main.cpp

	src/Vulkan/Utils/FileUtils.hpp
	src/Vulkan/Utils/Macros.hpp
	src/Vulkan/Utils/NoCopy.hpp
	src/Vulkan/Utils/ScopeGuard.cpp
	src/Vulkan/Utils/ScopeGuard.hpp
	src/Vulkan/Utils/SetOperations.hpp
	src/Vulkan/Utils/Version.hpp
	
	src/Vulkan/Device.cpp
	src/Vulkan/Device.hpp
	src/Vulkan/Forward.hpp
	src/Vulkan/GraphicsPipeline.cpp
	src/Vulkan/GraphicsPipeline.hpp
	src/Vulkan/Instance.cpp
	src/Vulkan/Instance.hpp
	src/Vulkan/PhysicalDevice.cpp
	src/Vulkan/PhysicalDevice.hpp
	src/Vulkan/PipelineLayout.cpp
	src/Vulkan/PipelineLayout.hpp
	src/Vulkan/QueueFamily.cpp
	src/Vulkan/QueueFamily.hpp
	src/Vulkan/ShaderStage.cpp
	src/Vulkan/ShaderStage.hpp
	src/Vulkan/SwapChain.cpp
	src/Vulkan/SwapChain.hpp
	src/Vulkan/WindowSurface.cpp
	src/Vulkan/WindowSurface.hpp
	
	src/VulkanOld/PhysicalDeviceID.cpp
	src/VulkanOld/PhysicalDeviceID.hpp
	src/VulkanOld/Queries.cpp
	src/VulkanOld/Queries.hpp
	src/VulkanOld/VulkanInstance.cpp
	src/VulkanOld/VulkanInstance.hpp
	src/VulkanOld/VulkanSettings.hpp
	src/VulkanOld/VulkanWindow.cpp
	src/VulkanOld/VulkanWindow.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add a macro:
#target_compile_definitions(${PROJECT_NAME} PRIVATE SOME_MACRO)


# Create the source code directory structure in visual studio.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX Source FILES ${SOURCES} ${HEADERS})

# Set as default startup item.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Set the current working directory to the runtime directory.
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/runtime")

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
