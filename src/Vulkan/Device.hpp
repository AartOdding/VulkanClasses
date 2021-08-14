#pragma once

#include <set>
#include <string>
#include <utility>
#include <vector>

#include <vulkan/vulkan.h>

#include <Vulkan/Forward.hpp>
#include <Vulkan/Utils/Macros.hpp>
#include <Vulkan/Utils/NoCopy.hpp>
#include <Vulkan/QueueFamily.hpp>


namespace Vulkan
{

	struct DeviceSettings
	{
		VkPhysicalDevice physicalDevice{};
		
		std::vector<std::pair<QueueRequirements, float>> requiredQueues{}; // QueueRequirements and priority (1.0f if you're not sure)
		std::vector<std::pair<QueueRequirements, float>> optionalQueues{}; // QueueRequirements and priority (1.0f if you're not sure)
		 
		std::set<std::string> requiredDeviceExtensions{};
		std::set<std::string> optionalDeviceExtensions{};

		VkPhysicalDeviceFeatures requiredDeviceFeatures{};
		VkPhysicalDeviceFeatures optionalDeviceFeatures{};
	};


	class Device
	{
	public:

		Device(DeviceSettings settings, const Instance* instance);
		~Device();

		const DeviceSettings& settings() const;

		const std::set<std::string>& deviceExtensions() const;
		const VkPhysicalDeviceFeatures& deviceFeatures() const;

		bool hasDeviceExtension(const std::string& deviceExtension) const;

		operator VkDevice() const;

		VULKAN_CLASSES_DISABLE_COPY_AND_MOVE(Device)

	private:

		VkDevice m_device;
		DeviceSettings m_settings;

		std::set<std::string> m_deviceExtensions;
		VkPhysicalDeviceFeatures m_deviceFeatures;
	};


}