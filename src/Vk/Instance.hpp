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
		std::string applicationName{};
		std::string engineName{};

		Utils::Version applicationVersion{ 1, 0, 0 };
		Utils::Version engineVersion{ 1, 0, 0 };

		Utils::Version vulkanVersion{ 1, 0, 0 };

		std::set<std::string> requiredInstanceExtensions{};
		std::set<std::string> optionalInstanceExtensions{};
		std::set<std::string> requiredValidationLayers{};
		std::set<std::string> optionalValidationLayers{};

		bool headless{ false };
	};


	class Instance : Utils::NoCopy
	{
	public:

		static std::vector<VkLayerProperties> availableValidationLayerProperties();
		static std::set<std::string> availableValidationLayerNames();

		static std::vector<VkExtensionProperties> availableInstanceExtensionProperties();
		static std::set<std::string> availableInstanceExtensionNames();


		Instance(InstanceSettings settings);

		VkInstance get() const;

		const std::string& applicationName() const;
		const std::string& engineName() const;

		Utils::Version applicationVersion() const;
		Utils::Version engineVersion() const;

		bool headless() const;

		std::vector<PhysicalDevice> availablePhysicalDevices() const;

		const std::vector<VkLayerProperties>& enabledValidationLayerProperties() const;
		const std::set<std::string>& enabledValidationLayerNames() const;

		const std::vector<VkExtensionProperties>& enabledInstanceExtensionProperties() const;
		const std::set<std::string>& enabledInstanceExtensionNames() const;

		bool isValidationLayerEnabled(const std::string& validationLayerName) const;
		bool isInstanceExtensionEnabled(const std::string& instanceExtensionName) const;

	private:

		VkInstance m_instance;

		Utils::ScopeGuard m_destructGlfw;
		Utils::ScopeGuard m_destructVulkan;

		std::vector<VkLayerProperties> m_enabledValidationLayerProperties;
		std::vector<VkExtensionProperties> m_enabledInstanceExtensionProperties;
		std::set<std::string> m_enabledInstanceExtensionNames;
		std::set<std::string> m_enabledValidationLayerNames;

		const std::string m_applicationName;
		const std::string m_engineName;

		const Utils::Version m_applicationVersion;
		const Utils::Version m_engineVersion;
		const Utils::Version m_vulkanVersion;

		const bool m_headless;
	};
}
