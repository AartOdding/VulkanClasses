#include "Utils/SetOperations.hpp"
#include "Vulkan/VulkanInstance.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>


namespace Vulkan
{

	VulkanInstance::VulkanInstance(const VulkanSettings& settings)
	{
		VulkanSettings newSettings = completeSettings(settings);

		initGlfw();
		createVulkanInstance(newSettings);
		pickPhysicalDevice(newSettings);

		createLogicalDevice();
	}

	VulkanInstance::~VulkanInstance()
	{

	}

	std::vector<VkLayerProperties> VulkanInstance::getAvailableValidationLayers()
	{
		std::vector<VkLayerProperties> availableLayers;

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		if (layerCount > 0)
		{
			availableLayers.resize(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		}
		return availableLayers;
	}

	std::vector<VkExtensionProperties> VulkanInstance::getAvailableInstanceExtensions()
	{
		std::vector<VkExtensionProperties> availableExtensions;

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		if (extensionCount > 0)
		{
			availableExtensions.resize(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
		}
		return availableExtensions;
	}

	std::unordered_set<std::string> VulkanInstance::getAvailableValidationLayerNames()
	{
		const auto layers = getAvailableValidationLayers();

		std::unordered_set<std::string> result;
		result.reserve(layers.size());

		for (const auto& layer : layers)
		{
			result.insert(layer.layerName);
		}
		return result;
	}

	std::unordered_set<std::string> VulkanInstance::getAvailableInstanceExtensionNames()
	{
		const auto extensions = getAvailableInstanceExtensions();

		std::unordered_set<std::string> result;
		result.reserve(extensions.size());

		for (const auto& extension : extensions)
		{
			result.insert(extension.extensionName);
		}
		return result;
	}

	std::vector<VkPhysicalDevice> VulkanInstance::getPhysicalDevices() const
	{
		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(m_vulkanInstance, &physicalDeviceCount, nullptr);

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);

		if (physicalDeviceCount > 0)
		{
			vkEnumeratePhysicalDevices(m_vulkanInstance, &physicalDeviceCount, physicalDevices.data());
		}
		return physicalDevices;
	}

	std::vector<VkQueueFamilyProperties> VulkanInstance::getQueueFamilies(VkPhysicalDevice device) const
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		
		if (queueFamilyCount > 0)
		{
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		}
		return queueFamilies;
	}

	VkPhysicalDevice VulkanInstance::getActivePhysicalDevice() const
	{
		return m_physicalDevice;
	}

	VulkanSettings VulkanInstance::completeSettings(const VulkanSettings& settings)
	{
		VulkanSettings newSettings = settings;

		auto glfwExtensionCount = 0u;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (int i = 0; i < glfwExtensionCount; ++i)
		{
			newSettings.requiredInstanceExtensions.insert(glfwExtensions[i]);
		}

#ifdef NDEBUG
		// In release mode, dont add validation layers
#else
		newSettings.optionalValidationLayers.insert("VK_LAYER_KHRONOS_validation");
#endif

		return newSettings;
	}

	bool VulkanInstance::comparePhysicalDevices(const VkPhysicalDevice& lhs, const VkPhysicalDevice& rhs)
	{
		std::cout << "test" << std::endl;
		return true;
	}

	void VulkanInstance::initGlfw()
	{
		if (glfwInit() == GLFW_TRUE)
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
	}

	void VulkanInstance::createVulkanInstance(const VulkanSettings& settings)
	{
		const auto availableValidationLayers = getAvailableValidationLayerNames();
		const auto availableInstanceExtensions = getAvailableInstanceExtensionNames();

		auto desiredValidationLayers = Utils::setUnion(
			settings.requiredValidationLayers, 
			settings.optionalValidationLayers);

		auto desiredInstanceExtensions = Utils::setUnion(
			settings.requiredInstanceExtensions,
			settings.optionalInstanceExtensions);

		const auto missingValidationLayers = Utils::setDifference(
			desiredValidationLayers, availableValidationLayers);

		const auto missingInstanceExtensions = Utils::setDifference(
			desiredInstanceExtensions, availableInstanceExtensions);

		for (const auto& missingLayer : missingValidationLayers)
		{
			if (settings.requiredValidationLayers.count(missingLayer))
			{
				throw std::runtime_error("Missing required validation layer: " + missingLayer);
			}
			else
			{
				std::cout << "Warning: Missing requested validation layer: " << missingLayer << std::endl;
				desiredValidationLayers.erase(missingLayer);
			}
		}

		for (const auto& missingExtension : missingInstanceExtensions)
		{
			if (settings.requiredInstanceExtensions.count(missingExtension))
			{
				throw std::runtime_error("Missing required instance extension: " + missingExtension);
			}
			else
			{
				std::cout << "Warning: Missing requested instance extension: " << missingExtension << std::endl;
				desiredInstanceExtensions.erase(missingExtension);
			}
		}

		std::vector<const char*> validationLayers;
		validationLayers.reserve(desiredValidationLayers.size());
		for (const auto& layer : desiredValidationLayers)
		{
			validationLayers.push_back(layer.c_str());
		}

		std::vector<const char*> instanceExtensions;
		instanceExtensions.reserve(desiredInstanceExtensions.size());
		for (const auto& extension : desiredInstanceExtensions)
		{
			instanceExtensions.push_back(extension.c_str());
		}

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
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
		createInfo.enabledExtensionCount = instanceExtensions.size();
		createInfo.ppEnabledExtensionNames = instanceExtensions.data();

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vulkanInstance);

		if (result == VK_SUCCESS)
		{
			m_destructVulkan = [this]()
			{
				std::cout << "Destroying Vulkan." << std::endl;
				vkDestroyInstance(m_vulkanInstance, nullptr);
			};
		}
		else
		{
			throw std::runtime_error("Failed to initialize Vulkan.");
		}
	}

	bool VulkanInstance::canPhysicalDevicePresent(VkPhysicalDevice device) const
	{
		auto queueFamilies = getQueueFamilies(device);

		for (int i = 0; i < queueFamilies.size(); ++i)
		{
			if (glfwGetPhysicalDevicePresentationSupport(m_vulkanInstance, device, i))
			{
				return true;
			}
		}
		return false;
	}

	bool VulkanInstance::isPhysicalDeviceSuitable(VkPhysicalDevice device, const VulkanSettings& settings) const
	{
		bool suitable = true;

		if (!settings.headlessOnly)
		{
			suitable &= canPhysicalDevicePresent(device);
		}
		return suitable;
	}

	std::vector<VkPhysicalDevice> VulkanInstance::getSuitablePhysicalDevices(VkInstance instance, const VulkanSettings& settings) const
	{
		std::vector<VkPhysicalDevice> allPhysicalDevices = getPhysicalDevices();
		std::vector<VkPhysicalDevice> suitablePhysicalDevices;
		suitablePhysicalDevices.reserve(allPhysicalDevices.size());

		for (const auto& device : allPhysicalDevices)
		{
			if (isPhysicalDeviceSuitable(device, settings))
			{
				suitablePhysicalDevices.push_back(device);
			}
		}
		return suitablePhysicalDevices;
	}

	void VulkanInstance::pickPhysicalDevice(const VulkanSettings& settings)
	{
		if (settings.physicalDeviceOverride)
		{
			// check if exists.
			// check if suitable
			// if yes, take it
			// if no give warning, and search for default
		}
		else
		{
			// take suitable devices
			// compare for best using comparison in settings
			// set it
		}

		auto suitableDevices = getSuitablePhysicalDevices(m_vulkanInstance, settings);

		// TODO: add option for override which device is used.

		// Try to find a discrete gpu
		for (const auto& device : suitableDevices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);


			PhysicalDeviceID id2{ deviceProperties };

			auto id3 = id2;

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				std::cout << "Using GPU: " << deviceProperties.deviceName << std::endl;
				m_physicalDevice = device;
				return;
			}
		}

		// Try to find integrated gpu
		for (const auto& device : suitableDevices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			{
				std::cout << "Using GPU: " << deviceProperties.deviceName << std::endl;
				m_physicalDevice = device;
				return;
			}
		}

		// Last resort: take anything.
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(suitableDevices[0], &deviceProperties);

		std::cout << "Using GPU: " << deviceProperties.deviceName << std::endl;
		m_physicalDevice = suitableDevices[0];
		return;
	}

	void VulkanInstance::findQueueFamilies(int& graphicsQueue)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

		// Find the first graphics queue:
		for (int i = 0; i < queueFamilyCount; ++i)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsQueue = i;
				return;
			}
		}
		throw std::runtime_error("Failed to find Vulkan graphics queue.");
	}

	void VulkanInstance::createLogicalDevice()
	{
		float graphicsqueuePriority = 1.0f;
		int graphicsQueue = -1;

		findQueueFamilies(graphicsQueue);

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = graphicsQueue;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &graphicsqueuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;

		createInfo.pEnabledFeatures = &deviceFeatures;
	}

}
