#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "Vulkan/Utils/FileUtils.hpp"
#include "Vulkan/Utils/NoCopy.hpp"
#include "Vulkan/Utils/ScopeGuard.hpp"

#include <Vulkan/GraphicsPipeline.hpp>
#include <Vulkan/Instance.hpp>
#include <Vulkan/LogicalDevice.hpp>
#include <Vulkan/WindowSurface.hpp>
#include <Vulkan/ShaderStage.hpp>
#include <Vulkan/SwapChain.hpp>


/*
void printQueues(const std::vector<Vulkan::PhysicalDevice>& physicalDevices, const Vulkan::WindowSurface& windowSurface)
{
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
}*/


int main()
{
    std::cout << "Current path: "  << std::filesystem::current_path() << std::endl;

    glfwSetErrorCallback([](int error, const char* description)
    {
        std::cerr << "GLFW error (" << error << ") " << description << std::endl;
    });

    if (!glfwInit())
    {
        return 1;
    }
    
    Vulkan::Utils::ScopeGuard terminateGlfw = []() {
        glfwTerminate();
    };

    Vulkan::InstanceSettings settings{};

    auto instance = std::make_unique<Vulkan::Instance>(settings);

    for (auto pd : instance->physicalDevices())
    {
        auto fams = pd.queueFamilies();

    }


    return 0; // =====================
    

    Vulkan::InstanceSettings instanceSettings;
    instanceSettings.optionalValidationLayers.insert("VK_LAYER_KHRONOS_validation");
    
    Vulkan::Instance vulkanInstance{ instanceSettings };
    Vulkan::WindowSurface windowSurface{ &vulkanInstance, {} };

    const auto physicalDevices = vulkanInstance.physicalDevices();
    //printQueues(physicalDevices, windowSurface);

    Vulkan::LogicalDeviceSettings logicalDeviceSettings;

    logicalDeviceSettings.physicalDevice = physicalDevices.at(0);
    logicalDeviceSettings.queuesToCreate = 
    { 
        { "transfer1", { 1, 0.9f } },
        { "compute2", { 2, 0.7f } },
        { "graphics1", { 0 } },
        { "transfer2", { 1, 0.1f } },
        { "compute1",  { 2, 0.01f } }
    };

    Vulkan::LogicalDevice logicalDevice{ &vulkanInstance, logicalDeviceSettings };

    auto swapChain = Vulkan::SwapChain(&logicalDevice, &windowSurface);

    auto graphicsPipeline = Vulkan::GraphicsPipeline(&logicalDevice, 100, 100);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
