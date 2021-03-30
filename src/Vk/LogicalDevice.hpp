#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <vulkan/vulkan.h>

#include <Utils/NoCopy.hpp>

#include <Vk/QueueFamily.hpp>
#include <Vk/PhysicalDevice.hpp>


namespace Vulkan
{
	class WindowSurface;

	struct QueueCreationSettings
	{
		int queueFamily = 0;
		float priority = 1.0f;
	};

	struct LogicalDeviceSettings
	{
		VkPhysicalDevice physicalDevice{};

		std::unordered_map<std::string, QueueCreationSettings> queuesToCreate{};
		 
		std::set<std::string> requiredDeviceExtensions{};
		std::set<std::string> optionalDeviceExtensions{};

		// Can split later in required/ optional
		VkPhysicalDeviceFeatures deviceFeatures{};
	};


	class LogicalDevice : Utils::NoCopy
	{
	public:

		LogicalDevice(const Instance* instance, LogicalDeviceSettings settings);
		~LogicalDevice();

		VkDevice get() const;

		const std::unordered_map<std::string, VkQueue>& queues() const;

		const std::vector<VkExtensionProperties>& enabledDeviceExtensionProperties() const;
		const std::set<std::string>& enabledDeviceExtensionNames() const;

		bool isDeviceExtensionEnabled(const std::string& deviceExtensionName) const;

	private:

		VkDevice m_logicalDevice;

		std::unordered_map<std::string, VkQueue> m_queues;
		std::set<std::string> m_enabledDeviceExtensionNames;
		std::vector<VkExtensionProperties> m_enabledDeviceExtensionProperties;

	};


}