#pragma once

#include <array>
#include <functional>
#include <optional>
#include <string>
#include <unordered_set>

#include <vulkan/vulkan.h>

#include "Utils/Version.hpp"
#include "Vulkan/PhysicalDeviceID.hpp"


namespace Vulkan
{

	struct VulkanSettings
	{
		std::string applicationName = "Application";
		std::string engineName = "Engine";

		Utils::Version applicationVersion{ 1, 0, 0 };
		Utils::Version engineVersion{ 1, 0, 0 };
		Utils::Version vulkanVersion{ 1, 0, 0 };

		std::unordered_set<std::string> deviceExtensions{};
		std::unordered_set<std::string> instanceExtensions{};
		std::unordered_set<std::string> validationLayers{};

		Vulkan::PhysicalDeviceID physicalDeviceOverride;
	};

}
