#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>

#include <Utils/SetOperations.hpp>
#include <Vk/Instance.hpp>


namespace Vulkan
{

	Instance::Instance(InstanceSettings settings)
		: m_applicationName(settings.applicationName)
		, m_engineName(settings.engineName)
		, m_applicationVersion(settings.applicationVersion)
		, m_engineVersion(settings.engineVersion)
		, m_headless(settings.headless)
	{
		// GLFW part:

		if (!settings.headless)
		{
			auto result = glfwInit();

			// Initialize glfw, and ensure cleanup:
			if (result == GLFW_TRUE)
			{
				m_destructGlfw = []()
				{
					std::cout << "Destroying glfw." << std::endl;
					glfwTerminate();
				};
			}
			else
			{
				throw std::runtime_error("Failed to initialize GLFW.");
			}

			// Add glfw's required instance extensions:
			auto glfwExtensionCount = 0u;
			auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			for (int i = 0; i < glfwExtensionCount; ++i)
			{
				settings.requiredInstanceExtensions.insert(glfwExtensions[i]);
			}
		}

		// Vulkan part:

		// Collect validation layers:

		const auto availableLayers = availableValidationLayerNames();
		const auto desiredLayers = Utils::setUnion(settings.requiredValidationLayers, settings.optionalValidationLayers);
		const auto missingLayers = Utils::setDifference(desiredLayers, availableLayers);

		for (const auto& missingLayer : missingLayers)
		{
			if (settings.requiredValidationLayers.count(missingLayer))
			{
				throw std::runtime_error("Missing required validation layer: " + missingLayer);
			}
			else
			{
				std::cerr << "[Warning] Missing optional validation layer: " << missingLayer << std::endl;
			}
		}

		m_enabledValidationLayerNames = Utils::setIntersection(desiredLayers, availableLayers);
		const auto layers = Utils::getCStringList(m_enabledValidationLayerNames);

		// Collect instance extensions:

		const auto availableExtensions = availableInstanceExtensionNames();
		const auto desiredExtensions = Utils::setUnion(settings.requiredInstanceExtensions, settings.optionalInstanceExtensions);
		const auto missingExtensions = Utils::setDifference(desiredExtensions, availableExtensions);

		for (const auto& missingExtension : missingExtensions)
		{
			if (settings.requiredInstanceExtensions.count(missingExtension))
			{
				throw std::runtime_error("Missing required instance extension: " + missingExtension);
			}
			else
			{
				std::cerr << "[Warning] Missing optional instance extension: " << missingExtension << std::endl;
			}
		}

		m_enabledInstanceExtensionNames = Utils::setIntersection(desiredExtensions, availableExtensions);
		const auto extensions = Utils::getCStringList(m_enabledInstanceExtensionNames);

		// Create the actual instance:

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = settings.applicationName.c_str();
		appInfo.applicationVersion = settings.applicationVersion.vkVersion();
		appInfo.pEngineName = settings.engineName.c_str();
		appInfo.engineVersion = settings.engineVersion.vkVersion();
		appInfo.apiVersion = settings.vulkanVersion.vkVersion();

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = layers.size();
		createInfo.ppEnabledLayerNames = layers.data();
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);

		if (result == VK_SUCCESS)
		{
			m_destructVulkan = [this]()
			{
				std::cout << "Destroying Vulkan." << std::endl;
				vkDestroyInstance(m_instance, nullptr);
			};
		}
		else
		{
			throw std::runtime_error("Failed to initialize Vulkan.");
		}

		// Fill in the enabled validation layer properties

		m_enabledValidationLayerProperties.reserve(m_enabledValidationLayerNames.size());
		for (const auto& properties : availableValidationLayerProperties())
		{
			if (m_enabledValidationLayerNames.count(properties.layerName))
			{
				m_enabledValidationLayerProperties.push_back(properties);
			}
		}

		// Fill in the enabled instance extension properties

		m_enabledInstanceExtensionProperties.reserve(m_enabledInstanceExtensionNames.size());
		for (const auto& properties : availableInstanceExtensionProperties())
		{
			if (m_enabledInstanceExtensionNames.count(properties.extensionName))
			{
				m_enabledInstanceExtensionProperties.push_back(properties);
			}
		}
	}

	VkInstance Instance::get() const
	{
		return m_instance;
	}

	const std::string& Instance::applicationName() const
	{
		return m_applicationName;
	}

	const std::string& Instance::engineName() const
	{
		return m_engineName;
	}

	Utils::Version Instance::applicationVersion() const
	{
		return m_applicationVersion;
	}

	Utils::Version Instance::engineVersion() const
	{
		return m_engineVersion;
	}

	bool Instance::headless() const
	{
		return m_headless;
	}

	std::vector<PhysicalDevice> Instance::availablePhysicalDevices() const
	{
		throw std::runtime_error("not implemented");
		return std::vector<PhysicalDevice>();
	}

	const std::vector<VkLayerProperties>& Instance::enabledValidationLayerProperties() const
	{
		return m_enabledValidationLayerProperties;
	}

	const std::set<std::string>& Instance::enabledValidationLayerNames() const
	{
		return m_enabledValidationLayerNames;
	}

	const std::vector<VkExtensionProperties>& Instance::enabledInstanceExtensionProperties() const
	{
		return m_enabledInstanceExtensionProperties;
	}

	const std::set<std::string>& Instance::enabledInstanceExtensionNames() const
	{
		return m_enabledInstanceExtensionNames;
	}

	bool Instance::isValidationLayerEnabled(const std::string& validationLayerName) const
	{
		throw std::runtime_error("not implemented");
		return true;
	}

	bool Instance::isInstanceExtensionEnabled(const std::string& instanceExtensionName) const
	{
		throw std::runtime_error("not implemented");
		return true;
	}

	std::vector<VkLayerProperties> Instance::availableValidationLayerProperties()
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

	std::set<std::string> Instance::availableValidationLayerNames()
	{
		const auto properties = availableValidationLayerProperties();

		std::set<std::string> names;

		for (const auto& layerProperties : properties)
		{
			names.insert(layerProperties.layerName);
		}
		return names;
	}

	std::vector<VkExtensionProperties> Instance::availableInstanceExtensionProperties()
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

	std::set<std::string> Instance::availableInstanceExtensionNames()
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
