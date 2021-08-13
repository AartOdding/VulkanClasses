#include "Queries.hpp"



namespace Vulkan
{

	std::vector<VkLayerProperties> getAvailableValidationLayers()
	{
		std::vector<VkLayerProperties> availableValidationLayers;

		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		if (count > 0)
		{
			availableValidationLayers.resize(count);
			vkEnumerateInstanceLayerProperties(&count, availableValidationLayers.data());
		}
		return availableValidationLayers;
	}

	std::vector<VkExtensionProperties> getAvailableInstanceExtensions()
	{
		std::vector<VkExtensionProperties> availableInstanceExtensions;

		uint32_t count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

		if (count > 0)
		{
			availableInstanceExtensions.resize(count);
			vkEnumerateInstanceExtensionProperties(nullptr, &count, availableInstanceExtensions.data());
		}
		return availableInstanceExtensions;
	}

	std::vector<VkExtensionProperties> getAvailableDeviceExtensions(VkPhysicalDevice device)
	{
		std::vector<VkExtensionProperties> availableDeviceExtensions;

		uint32_t count = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

		if (count > 0)
		{
			availableDeviceExtensions.resize(count);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &count, availableDeviceExtensions.data());
		}
		return availableDeviceExtensions;
	}

	std::unordered_set<std::string> getAvailableValidationLayerNames()
	{
		const auto validationLayers = getAvailableValidationLayers();

		std::unordered_set<std::string> validationLayerNames;
		validationLayerNames.reserve(validationLayers.size());

		for (const auto& validationLayer : validationLayers)
		{
			validationLayerNames.insert(validationLayer.layerName);
		}
		return validationLayerNames;
	}

	std::unordered_set<std::string> getAvailableInstanceExtensionNames()
	{
		const auto instanceExtensions = getAvailableInstanceExtensions();

		std::unordered_set<std::string> instanceExtensionNames;
		instanceExtensionNames.reserve(instanceExtensions.size());

		for (const auto& instanceExtension : instanceExtensions)
		{
			instanceExtensionNames.insert(instanceExtension.extensionName);
		}
		return instanceExtensionNames;
	}

	std::unordered_set<std::string> getAvailableDeviceExtensionNames(VkPhysicalDevice device)
	{
		const auto deviceExtensions = getAvailableDeviceExtensions(device);

		std::unordered_set<std::string> deviceExtensionNames;
		deviceExtensionNames.reserve(deviceExtensions.size());

		for (const auto& deviceExtension : deviceExtensions)
		{
			deviceExtensionNames.insert(deviceExtension.extensionName);
		}
		return deviceExtensionNames;
	}

	std::vector<VkPhysicalDevice> getAvailablePhysicalDevices(VkInstance instance)
	{
		std::vector<VkPhysicalDevice> availablePhysicalDevices;

		uint32_t count = 0;
		vkEnumeratePhysicalDevices(instance, &count, nullptr);

		if (count > 0)
		{
			availablePhysicalDevices.resize(count);
			vkEnumeratePhysicalDevices(instance, &count, availablePhysicalDevices.data());
		}
		return availablePhysicalDevices;
	}

	VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);
		return properties;
	}

	std::vector<VkQueueFamilyProperties> getAvailableQueueFamilies(VkPhysicalDevice device)
	{
		std::vector<VkQueueFamilyProperties> queueFamilies;

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

		if (count > 0)
		{
			queueFamilies.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies.data());
		}
		return queueFamilies;
	}
}
