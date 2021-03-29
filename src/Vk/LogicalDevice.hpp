#pragma once

#include <set>
#include <string>
#include <utility>
#include <vector>

#include <vulkan/vulkan.h>

#include <Utils/NoCopy.hpp>

#include <Vk/QueueFamily.hpp>
#include <Vk/PhysicalDevice.hpp>


namespace Vulkan
{

	class QueueCreationSettings
	{
	public:

		QueueCreationSettings(QueueFlags requiredFlags, int count = 1) {}
		QueueCreationSettings(const QueueFamily& family, int count = 1) {}

	};

	struct LogicalDeviceSettings
	{
		PhysicalDevice physicalDevice{};

		std::vector<QueueCreationSettings> queues{};

		std::set<std::string> requiredInstanceExtensions{};
		std::set<std::string> optionalInstanceExtensions{};

		// maybe split optional and required if possible? 
		VkPhysicalDeviceFeatures physicalDeviceFeatures{};
	};


	class LogicalDevice : Utils::NoCopy
	{
	public:

		LogicalDevice(LogicalDeviceSettings settings);
		~LogicalDevice();

	private:


	};


}