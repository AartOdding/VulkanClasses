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
#include <Vulkan/Device.hpp>
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

    Vulkan::InstanceSettings isettings{};

    auto instance = std::make_unique<Vulkan::Instance>(isettings);

    Vulkan::DeviceSettings dsettings;
    dsettings.physicalDevice = instance->physicalDevices()[0];
    dsettings.requiredQueues.push_back({ { VK_QUEUE_GRAPHICS_BIT }, 1.0f });
    dsettings.optionalQueues.push_back({ { VK_QUEUE_COMPUTE_BIT }, 1.0f });
    dsettings.optionalQueues.push_back({ { VK_QUEUE_TRANSFER_BIT }, 1.0f });

    auto device = std::make_unique<Vulkan::Device>(dsettings, instance.get());




    
    auto resolved = instance->physicalDevices()[0].resolveQueues({ { VK_QUEUE_GRAPHICS_BIT }, { VK_QUEUE_COMPUTE_BIT }, { VK_QUEUE_TRANSFER_BIT } });


    return 0; // =====================
    
}
