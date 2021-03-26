#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Utils/NoCopy.hpp>
#include <Utils/ScopeGuard.hpp>


namespace Vulkan
{
	class VulkanInstance;


	class VulkanWindow : Utils::NoCopy
	{
	public:

		VulkanWindow(const VulkanInstance& vulkanInstance);
		~VulkanWindow();

		void update();

	private:

		GLFWwindow* m_window;
		VkSurfaceKHR m_surface;

		Utils::ScopeGuard m_windowCleanup;
		Utils::ScopeGuard m_surfaceCleanup;

	};
}
