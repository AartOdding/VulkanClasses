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
#include <Vk/SwapChain.hpp>



int main()
{
    Vulkan::InstanceSettings instanceSettings;
    instanceSettings.optionalValidationLayers.insert("VK_LAYER_KHRONOS_validation");
    
    Vulkan::Instance vulkanInstance{ instanceSettings };
    Vulkan::WindowSurface windowSurface{ &vulkanInstance, {} };

    const auto physicalDevices = vulkanInstance.availablePhysicalDevices();
    for (const auto& device : physicalDevices)
    {
        const auto queueFamilies = device.availableQueueFamilyProperties();

        for (int i = 0; i < queueFamilies.size(); ++i)
        {
            VkBool32 presentSupport = false;

            vkGetPhysicalDeviceSurfaceSupportKHR(device.get(), i, windowSurface.surface(), &presentSupport);

            std::cout << "Queue family index: " << i 
                << ", max count: " << queueFamilies[i].queueCount 
                << ", can present: " << presentSupport << std::endl;
        }
    }

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

    Vulkan::LogicalDevice logicalDevice{ &vulkanInstance, logicalDeviceSettings };

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevices.at(0).get(), windowSurface.surface(), &surfaceCapabilities);

    auto formats = Vulkan::SwapChain::supportedFormats(logicalDevice, windowSurface);

    auto swapChain = Vulkan::SwapChain(&logicalDevice, &windowSurface);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
