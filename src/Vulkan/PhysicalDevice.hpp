#pragma once

#include <set>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <Vulkan/Forward.hpp>
#include <Vulkan/QueueFamily.hpp>
#include <Vulkan/Utils/Macros.hpp>


namespace Vulkan
{

	class PhysicalDevice
	{
	public:

		PhysicalDevice();
		PhysicalDevice(VkPhysicalDevice device);

		std::vector<QueueFamily> queueFamilies() const;

		std::set<std::string> availableDeviceExtensions() const;

		operator VkPhysicalDevice() const;

		VULKAN_CLASSES_DEFAULT_COPY_AND_MOVE(PhysicalDevice)

	private:

		VkPhysicalDevice m_device;

	};

}
