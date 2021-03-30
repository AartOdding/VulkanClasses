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
    const auto physicalDevices = vulkanInstance.availablePhysicalDevices();

    for (const auto& device : physicalDevices)
    {
        const auto queueFamilies = device.availableQueueFamilyProperties();

        for (int i = 0; i < queueFamilies.size(); ++i)
        {
            std::cout << "Queue family: " << i << ", count: " << queueFamilies[i].queueCount << std::endl;
        }
    }

    Vulkan::WindowSurface vulkanWindow{ &vulkanInstance, {} };


    Vulkan::LogicalDeviceSettings logicalDeviceSettings;

    logicalDeviceSettings.physicalDevice = physicalDevices.at(0).get();
    logicalDeviceSettings.queuesToCreate = 
    { 
        { "transfer1", { 1, 0.9f } },
        { "compute2", { 2, 0.7f } },
        { "graphics1", { 0 } },
        { "transfer2", { 1, 0.1f } },
        { "compute1",  { 2, 0.01f } }
    };
    logicalDeviceSettings.optionalDeviceExtensions.insert("gahaha");
    logicalDeviceSettings.optionalDeviceExtensions.insert("ewf");
    logicalDeviceSettings.requiredDeviceExtensions.insert(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    Vulkan::LogicalDevice logicalDevice{ &vulkanInstance, logicalDeviceSettings };

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
