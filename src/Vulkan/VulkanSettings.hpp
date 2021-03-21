#pragma once

#include <string>
#include <unordered_set>

#include "Utils/Version.hpp"


namespace Vulkan
{

	struct VulkanSettings
	{
		std::string applicationName = "Application";
		std::string engineName = "Engine";

		Utils::Version applicationVersion{ 1, 0, 0 };
		Utils::Version engineVersion{ 1, 0, 0 };
		Utils::Version vulkanVersion{ 1, 0, 0 };

		std::unordered_set<std::string> requiredDeviceExtensions{};
		std::unordered_set<std::string> optionalDeviceExtensions{};

		std::unordered_set<std::string> requiredInstanceExtensions{};
		std::unordered_set<std::string> optionalInstanceExtensions{};

		std::unordered_set<std::string> requiredValidationLayers{};
		std::unordered_set<std::string> optionalValidationLayers{};
	};

}
