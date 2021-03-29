#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>
#include <thread>

#include "Application.hpp"
#include "Utils/Assert.hpp"
#include "Utils/NoCopy.hpp"
#include "Utils/RaiiWrapper.hpp"
#include "Utils/ScopeGuard.hpp"

#include <Vk/Instance.hpp>
#include <Vk/LogicalDevice.hpp>
#include <Vk/WindowSurface.hpp>



int main()
{
    Vulkan::InstanceSettings instanceSettings;
    instanceSettings.optionalValidationLayers.insert("VK_LAYER_KHRONOS_validation");
    
    Vulkan::Instance vulkanInstance{ instanceSettings };
    auto physicalDevices = vulkanInstance.availablePhysicalDevices();

    Vulkan::WindowSurface vulkanWindow{ &vulkanInstance, {} };


    Vulkan::LogicalDeviceSettings logicalDeviceSettings;

    logicalDeviceSettings.physicalDevice = physicalDevices.at(0).get();
    logicalDeviceSettings.queuesToCreate = { { 0 } };

    Vulkan::LogicalDevice logicalDevice{ logicalDeviceSettings };

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
