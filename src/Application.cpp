#include "Application.hpp"

#include <iostream>
#include <stdexcept>



Application* Application::g_instance = nullptr;

Application* Application::instance()
{
	return g_instance;
}


Application::Application()
{
	initWindow();
	initVulkan();
	g_instance = this;
}


Application::~Application()
{
	g_instance = nullptr;
}

void Application::initWindow()
{
	if (glfwInit() == GLFW_TRUE)
	{
		m_destructGlfw = []()
		{
			std::cout << "Destroying glfw." << std::endl;
			glfwTerminate();
		};
	}
	else
	{
		throw std::runtime_error("Failed to initialize GLFW.");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(900, 900, "Vulkan", nullptr, nullptr);

	if (m_window)
	{
		m_destructWindow = [this]()
		{
			std::cout << "Destroying window." << std::endl;
			glfwDestroyWindow(m_window);
		};
	}
	else
	{
		throw std::runtime_error("Failed to create window.");
	}
}

void Application::initVulkan()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "BlockCraft";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "BlockCraft";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = 0;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensionNames;
	glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensionNames;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vulkanInstance);

	if (result == VK_SUCCESS)
	{
		m_destructVulkan = [this]()
		{
			std::cout << "Destroying Vulkan." << std::endl;
			vkDestroyInstance(m_vulkanInstance, nullptr);
		};
	}
	else
	{
		throw std::runtime_error("Failed to initialize Vulkan.");
	}
}

void Application::run()
{

}
