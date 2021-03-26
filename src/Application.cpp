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

	Vulkan::VulkanSettings settings;

	m_vulkan = std::make_unique<Vulkan::VulkanInstance>(settings);
	m_window = std::make_unique<Vulkan::VulkanWindow>(*m_vulkan);


	g_instance = this;
}


Application::~Application()
{
	g_instance = nullptr;
}



void Application::run()
{

}
