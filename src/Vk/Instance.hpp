#pragma once

#include <set>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <Utils/NoCopy.hpp>
#include <Utils/ScopeGuard.hpp>
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

		std::set<std::string> requiredInstanceExtensions{};
		std::set<std::string> optionalinstanceExtensions{};
		std::set<std::string> requiredValidationLayers{};
		std::set<std::string> optionalValidationLayers{};

		bool headless{ false };
	};


	class Instance : Utils::NoCopy
	{
	public:

		static std::vector<VkLayerProperties> availableValidationLayers();
		static std::set<std::string> availableValidationLayerNames();

		static std::vector<VkExtensionProperties> availableInstanceExtensions();
		static std::set<std::string> availableInstanceExtensionNames();


		Instance(const InstanceSettings& settings);

		VkInstance get() const;

		const std::string& applicationName() const;
		const std::string& engineName() const;

		Utils::Version applicationVersion() const;
		Utils::Version engineVersion() const;

		bool headless() const;

		std::vector<PhysicalDevice> availablePhysicalDevices() const;

		const std::vector<VkLayerProperties>& enabledValidationLayers() const;
		const std::set<std::string>& enabledValidationLayerNames() const;

		const std::vector<VkExtensionProperties>& enabledInstanceExtensions() const;
		const std::set<std::string>& enabledInstanceExtensionNames() const;

		bool isValidationLayerEnabled(const std::string& validationLayerName) const;
		bool isInstanceExtensionEnabled(const std::string& instanceExtensionName) const;

	private:

		VkInstance m_instance;

		Utils::ScopeGuard m_destructGlfw;
		Utils::ScopeGuard m_destructVulkan;

		std::vector<VkLayerProperties> m_enabledValidationLayers;
		std::vector<VkExtensionProperties> m_enabledInstanceExtensions;
		std::set<std::string> m_enabledInstanceExtensionNames;
		std::set<std::string> m_enabledValidationLayerNames;

		std::string applicationName;
		std::string engineName;

		Utils::Version m_applicationVersion;
		Utils::Version m_engineVersion;

		bool m_headless;
	};
}
