#include "Vulkan/VulkanWindow.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


namespace Vulkan
{

	VulkanWindow::VulkanWindow()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		auto window = glfwCreateWindow(900, 900, "Vulkan", nullptr, nullptr);

		VkSurfaceKHR surface;
		//VkResult err = glfwCreateWindowSurface(m_vulkanInstance, window, nullptr, &surface);
		/*
		if (err)
		{
			// Window surface creation failed
		}*/
	}

	VulkanWindow::~VulkanWindow()
	{

	}

}
