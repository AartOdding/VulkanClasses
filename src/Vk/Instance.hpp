#pragma once

#include <set>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <Utils/NoCopy.hpp>
#include <Utils/Version.hpp>

#include <Vk/PhysicalDevice.hpp>


namespace Vulkan
{

	struct InstanceSettings
	{
		std::string applicationName;
		std::string engineName;

		Utils::Version applicationVersion{ 1, 0, 0 };
		Utils::Version engineVersion{ 1, 0, 0 };

		std::set<std::string> instanceExtensions{};
		std::set<std::string> validationLayers{};

		bool headless{ false };
	};

	// In charge of glfw and vulkan!
	class Instance : Utils::NoCopy
	{
	public:

		Instance(const InstanceSettings& settings);
		~Instance();

		VkInstance get() const;

		const InstanceSettings& settings() const;

		std::vector<PhysicalDevice> availablePhysicalDevices() const;

	private:

		VkInstance m_instance;
		const InstanceSettings m_settings;

	};
}
