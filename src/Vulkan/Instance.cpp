#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>

#include <Vulkan/Utils/SetOperations.hpp>
#include <Vulkan/Instance.hpp>


namespace Vulkan
{

	Instance::Instance(InstanceSettings settings)
		: m_settings(settings)
	{
		// Collect validation layers:

		const auto availableLayers = availableValidationLayers();
		const auto desiredLayers = Utils::setUnion(m_settings.requiredValidationLayers, m_settings.optionalValidationLayers);
		const auto missingLayers = Utils::setDifference(desiredLayers, availableLayers);

		for (const auto& missingLayer : missingLayers)
		{
			if (m_settings.requiredValidationLayers.count(missingLayer))
			{
				throw std::runtime_error("Missing required validation layer: " + missingLayer);
			}
		}

		m_validationLayers = Utils::setIntersection(desiredLayers, availableLayers);
		const auto layers = Utils::getCStringList(m_validationLayers);

		// Collect instance extensions:

		const auto availableExtensions = availableInstanceExtensions();
		const auto desiredExtensions = Utils::setUnion(m_settings.requiredInstanceExtensions, m_settings.optionalInstanceExtensions);
		const auto missingExtensions = Utils::setDifference(desiredExtensions, availableExtensions);

		for (const auto& missingExtension : missingExtensions)
		{
			if (m_settings.requiredInstanceExtensions.count(missingExtension))
			{
				throw std::runtime_error("Missing required instance extension: " + missingExtension);
			}
		}

		m_instanceExtensions = Utils::setIntersection(desiredExtensions, availableExtensions);
		const auto extensions = Utils::getCStringList(m_instanceExtensions);

		// Create the actual instance:

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = m_settings.applicationName.c_str();
		appInfo.applicationVersion = getVulkanVersion(m_settings.applicationVersion);
		appInfo.pEngineName = m_settings.engineName.c_str();
		appInfo.engineVersion = getVulkanVersion(m_settings.engineVersion);
		appInfo.apiVersion = getVulkanVersion(m_settings.vulkanVersion);

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = layers.size();
		createInfo.ppEnabledLayerNames = layers.data();
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to initialize Vulkan.");
		}
	}

	Instance::~Instance()
	{
		vkDestroyInstance(m_instance, nullptr);
	}

	const InstanceSettings& Instance::settings() const
	{
		return m_settings;
	}

	const std::set<std::string>& Instance::validationLayers() const
	{
		return m_validationLayers;
	}

	const std::set<std::string>& Instance::instanceExtensions() const
	{
		return m_instanceExtensions;
	}

	bool Instance::hasValidationLayer(const std::string& validationLayerName) const
	{
		return m_validationLayers.count(validationLayerName);
	}

	bool Instance::hasInstanceExtension(const std::string& instanceExtensionName) const
	{
		return m_instanceExtensions.count(instanceExtensionName);
	}

	Instance::operator VkInstance() const
	{
		return m_instance;
	}

	std::vector<PhysicalDevice> Instance::physicalDevices() const
	{
		std::vector<VkPhysicalDevice> vulkanDevices;
		std::vector<PhysicalDevice> devices;

		uint32_t count = 0;
		vkEnumeratePhysicalDevices(m_instance, &count, nullptr);

		if (count > 0)
		{
			vulkanDevices.resize(count);
			vkEnumeratePhysicalDevices(m_instance, &count, vulkanDevices.data());

			devices.reserve(count);
			for (const auto& vkDevice : vulkanDevices)
			{
				devices.emplace_back(vkDevice);
			}
		}

		return devices;
	}

	static std::vector<VkLayerProperties> availableValidationLayerProperties()
	{
		std::vector<VkLayerProperties> properties;

		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		if (count > 0)
		{
			properties.resize(count);
			vkEnumerateInstanceLayerProperties(&count, properties.data());
		}
		return properties;
	}

	std::set<std::string> Instance::availableValidationLayers()
	{
		const auto properties = availableValidationLayerProperties();

		std::set<std::string> names;

		for (const auto& layerProperties : properties)
		{
			names.insert(layerProperties.layerName);
		}
		return names;
	}

	static std::vector<VkExtensionProperties> availableInstanceExtensionProperties()
	{
		std::vector<VkExtensionProperties> properties;

		uint32_t count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

		if (count > 0)
		{
			properties.resize(count);
			vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());
		}
		return properties;
	}

	std::set<std::string> Instance::availableInstanceExtensions()
	{
		const auto properties = availableInstanceExtensionProperties();

		std::set<std::string> names;

		for (const auto& extensionProperties : properties)
		{
			names.insert(extensionProperties.extensionName);
		}
		return names;
	}

}
