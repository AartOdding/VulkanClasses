#pragma once

#include <set>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <Vulkan/Utils/Macros.hpp>
#include <Vulkan/Utils/Version.hpp>

#include <Vulkan/PhysicalDevice.hpp>


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
	};


	class Instance
	{
	public:

		static std::set<std::string> availableValidationLayers();
		static std::set<std::string> availableInstanceExtensions();

		Instance(InstanceSettings settings);
		~Instance();

		const InstanceSettings& settings() const;

		const std::set<std::string>& validationLayers() const;
		const std::set<std::string>& instanceExtensions() const;

		bool hasValidationLayer(const std::string& validationLayer) const;
		bool hasInstanceExtension(const std::string& instanceExtension) const;

		std::vector<PhysicalDevice> physicalDevices() const;

		operator VkInstance() const;

		VULKAN_CLASSES_DISABLE_COPY_AND_MOVE(Instance)

	private:

		VkInstance m_instance;
		InstanceSettings m_settings;

		std::set<std::string> m_instanceExtensions;
		std::set<std::string> m_validationLayers;

	};
}
