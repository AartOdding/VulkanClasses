#include <iostream>
#include <stdexcept>

#include <Vulkan/Instance.hpp>
#include <Vulkan/WindowSurface.hpp>

#include <GLFW/glfw3.h>


namespace Vulkan
{

	WindowSurface::WindowSurface(const Instance* instance, WindowSettings settings)
	{
		if (!instance)
		{
			throw std::runtime_error("Cannot create window surface without Instance.");
		}
		if (instance->headless())
		{
			throw std::runtime_error("Cannot create window surface for headless Instance.");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_window = glfwCreateWindow(settings.initialWidth, settings.initialHeight, 
			settings.windowTitle.c_str(), nullptr, nullptr);

		if (m_window)
		{
			m_windowCleanup = [window = m_window]()
			{
				std::cout << "Destroying Window." << std::endl;
				glfwDestroyWindow(window);
			};
		}
		else
		{
			throw std::runtime_error("Failed to create window!");
		}

		auto result = glfwCreateWindowSurface(instance->get(), m_window, nullptr, &m_surface);

		if (result == VK_SUCCESS)
		{
			m_surfaceCleanup = [instance = instance->get(), surface = m_surface]()
			{
				std::cout << "Destroying Surface." << std::endl;
				vkDestroySurfaceKHR(instance, surface, nullptr);
			};
		}
		else
		{
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	VkSurfaceKHR WindowSurface::surface() const
	{
		return m_surface;
	}

	GLFWwindow* WindowSurface::window() const
	{
		return m_window;
	}

	VkExtent2D WindowSurface::frameBufferSize() const
	{
		int x, y;
		glfwGetFramebufferSize(m_window, &x, &y);
		return { static_cast<uint32_t>(x), static_cast<uint32_t>(y) };
	}
}
