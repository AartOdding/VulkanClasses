#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>

#include "Application.hpp"
#include "Utils/Assert.hpp"
#include "Utils/NoCopy.hpp"
#include "Utils/RaiiWrapper.hpp"
#include "Utils/ScopeGuard.hpp"

#include <Vk/Instance.hpp>
#include <Vk/LogicalDevice.hpp>



int main()
{
    Vulkan::InstanceSettings instanceSettings;
    instanceSettings.optionalValidationLayers.insert("VK_LAYER_KHRONOS_validation");
    
    Vulkan::Instance vulkanInstance{ instanceSettings };

    auto devices = vulkanInstance.availablePhysicalDevices();

    auto queueFamilies = devices.at(0).availableQueueFamilies();

    Vulkan::LogicalDeviceSettings logicalDeviceSettings;

    logicalDeviceSettings.queues =
    {
        { Vulkan::QueueFlags::Graphics | Vulkan::QueueFlags::Compute },
        { Vulkan::QueueFlags::Transfer }
    };

    Vulkan::LogicalDevice logicalDevice{ logicalDeviceSettings };

    return 0;
}
