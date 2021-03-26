#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#include "Vulkan/VulkanInstance.hpp"
#include "Vulkan/VulkanWindow.hpp"



class Application
{
public:

	static Application* instance();

	Application();
	~Application();

	void run();

private:

	std::unique_ptr<Vulkan::VulkanInstance> m_vulkan;
	std::unique_ptr<Vulkan::VulkanWindow> m_window;

	static Application* g_instance;
};
