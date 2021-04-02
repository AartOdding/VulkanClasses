#pragma once

#include <string>

#include <vulkan/vulkan.h>

#include "Utils/NoCopy.hpp"
#include "Utils/ScopeGuard.hpp"



struct GLFWwindow;


namespace Vulkan
{
	class Instance;


	struct WindowSettings
	{
		std::string windowTitle = "Vulkan";
		int initialWidth = 1280;
		int initialHeight = 720;
	};


	class WindowSurface : Utils::NoCopy
	{
	public:

		WindowSurface(const Instance* instance, WindowSettings settings);

		VkSurfaceKHR surface() const;
		GLFWwindow* window() const;

		VkExtent2D frameBufferSize() const;

	private:

		Utils::ScopeGuard m_windowCleanup;
		Utils::ScopeGuard m_surfaceCleanup;

		GLFWwindow* m_window;
		VkSurfaceKHR m_surface;

	};

}
