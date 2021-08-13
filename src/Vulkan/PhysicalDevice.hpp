#pragma once

#include <set>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <Vulkan/Utils/NoCopy.hpp>


namespace Vulkan
{
	class Instance;

	class PhysicalDevice
	{
	public:

		PhysicalDevice();
		PhysicalDevice(VkPhysicalDevice device);

		VkPhysicalDevice get() const;

		std::vector<VkQueueFamilyProperties> availableQueueFamilyProperties() const;
		std::vector<VkExtensionProperties> availableDeviceExtensionProperties() const;
		std::set<std::string> availableDeviceExtensionNames() const;

	private:

		VkPhysicalDevice m_device;

	};
}
