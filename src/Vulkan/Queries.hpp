#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.h>


// queries for static data only, nothing needs to be freed
// no vulkan extensions or glfw

namespace Vulkan
{

	std::vector<VkLayerProperties> getAvailableValidationLayers();
	std::unordered_set<std::string> getAvailableValidationLayerNames();

	std::vector<VkExtensionProperties> getAvailableInstanceExtensions();
	std::unordered_set<std::string> getAvailableInstanceExtensionNames();

	std::vector<VkPhysicalDevice> getAvailablePhysicalDevices(VkInstance instance);

	std::vector<VkExtensionProperties> getAvailableDeviceExtensions(VkPhysicalDevice device);
	std::unordered_set<std::string> getAvailableDeviceExtensionNames(VkPhysicalDevice device);

	std::vector<VkQueueFamilyProperties> getAvailableQueueFamilies(VkPhysicalDevice device);

	VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);

}


namespace Vulkan
{
	VkSurfaceCapabilitiesKHR getSurfaceCapabilities(VkPhysicalDevice device, VkSurfaceKHR surface);

	std::vector<VkSurfaceFormatKHR> getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface);
	std::vector<VkPresentModeKHR> getSurfacePresentModes(VkPhysicalDevice device, VkSurfaceKHR surface);

}
