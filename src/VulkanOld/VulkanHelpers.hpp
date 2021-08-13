#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>


namespace Vulkan
{

	static std::vector<VkLayerProperties> getAvailableValidationLayers();
	static std::vector<VkExtensionProperties> getAvailableInstanceExtensions();

	static std::unordered_set<std::string> getAvailableValidationLayerNames();
	static std::unordered_set<std::string> getAvailableInstanceExtensionNames();

}
