#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#include "Utils/DestructCall.hpp"
#include "Vulkan/VulkanInstance.hpp"



class Application
{
public:

	static Application* instance();

	Application();
	~Application();

	void run();

private:

	void initWindow();

	// Destructed in reverse order:
	Utils::DestructCall m_destructWindow;

	GLFWwindow* m_window;
	std::unique_ptr<Vulkan::VulkanInstance> m_vulkan;

	static Application* g_instance;
};
