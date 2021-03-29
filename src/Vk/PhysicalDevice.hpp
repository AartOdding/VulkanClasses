#pragma once

#include <vector>

#include <Utils/NoCopy.hpp>

#include <vulkan/vulkan.h>

#include <Vk/QueueFamily.hpp>


namespace Vulkan
{
	class Instance;

	class PhysicalDevice
	{
	public:

		PhysicalDevice();
		PhysicalDevice(const Instance* vulkanInstance, VkPhysicalDevice device);

		VkPhysicalDevice get() const;
		const Instance* vulkanInstance() const;

		std::vector<QueueFamily> availableQueueFamilies();

	private:

		VkPhysicalDevice m_device;
		const Instance* m_instance;

	};
}
