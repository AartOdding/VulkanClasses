#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>

#include "Application.hpp"
#include "Utils/DestructCall.hpp"

namespace
{
    void initWindow()
    {

    }

}

/*
int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
*/

int main()
{
    try
    {
        glfwInit();

        Utils::DestructCall destructGlfw = []()
        {
            glfwTerminate();
        };

        Application application;

        Utils::DestructCall d1 = []() { std::cout << "hello d1" << std::endl; };
        Utils::DestructCall d2{ []() { std::cout << "hello d2" << std::endl; } };
    }
    catch (const std::exception& e)
    {
        std::cerr << "Uncaught exception:\n" << e.what() << std::endl;
        return 1;
    }
    return 0;
}
