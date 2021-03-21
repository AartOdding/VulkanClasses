#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Utils/DestructCall.hpp"


class Application
{
public:

	static Application* instance();

	Application();
	~Application();

	void run();

private:

	void initWindow();
	void initVulkan();

	// Destructed in reverse order:
	Utils::DestructCall m_destructGlfw;
	Utils::DestructCall m_destructWindow;
	Utils::DestructCall m_destructVulkan;

	GLFWwindow* m_window;
	VkInstance m_vulkanInstance;

	static Application* g_instance;
};
