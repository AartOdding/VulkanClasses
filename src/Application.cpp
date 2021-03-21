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

	Vulkan::VulkanSettings settings;

	m_vulkan = std::make_unique<Vulkan::VulkanInstance>(settings);

	g_instance = this;
}


Application::~Application()
{
	g_instance = nullptr;
}

void Application::initWindow()
{

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


void Application::run()
{

}
