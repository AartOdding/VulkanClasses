#include "Vulkan/VulkanWindow.hpp"
#include "Vulkan/VulkanInstance.hpp"

#include <iostream>
#include <stdexcept>


namespace Vulkan
{

	VulkanWindow::VulkanWindow(const VulkanInstance& vulkanInstance)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_window = glfwCreateWindow(900, 900, "Vulkan", nullptr, nullptr);

		if (m_window)
		{
			m_windowCleanup = [window = m_window]()
			{
				std::cout << "Destroying window." << std::endl;
				glfwDestroyWindow(window);
			};
		}
		else
		{
			throw std::runtime_error("Failed to create window!");
		}
		
		auto result = glfwCreateWindowSurface(vulkanInstance.getVkInstance(), m_window, nullptr, &m_surface);

		if (result == VK_SUCCESS)
		{
			m_surfaceCleanup = [instance = vulkanInstance.getVkInstance(), surface = m_surface]()
			{
				std::cout << "Destroying surface." << std::endl;
				vkDestroySurfaceKHR(instance, surface, nullptr);
			};
		}
		else
		{
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	VulkanWindow::~VulkanWindow()
	{

	}


	void VulkanWindow::update()
	{

	}

}
