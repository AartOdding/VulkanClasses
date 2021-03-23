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
		createLogicalDevice(newSettings);
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

	VkPhysicalDevice VulkanInstance::getActivePhysicalDevice()
	{
		return m_physicalDevice;
	}

	VkQueue VulkanInstance::getMainCommandQueue()
	{
		return m_mainCommandQueue;
	}

	VulkanSettings VulkanInstance::completeSettings(const VulkanSettings& settings)
	{
		VulkanSettings newSettings = settings;

		auto glfwExtensionCount = 0u;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (int i = 0; i < glfwExtensionCount; ++i)
		{
			newSettings.instanceExtensions.insert(glfwExtensions[i]);
		}

#ifdef NDEBUG
		// In release mode, dont add validation layers
#else
		newSettings.validationLayers.insert("VK_LAYER_KHRONOS_validation");
#endif

		return newSettings;
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
		
		std::vector<const char*> validationLayers;
		validationLayers.reserve(settings.validationLayers.size());

		for (const auto& layer : settings.validationLayers)
		{
			if (availableValidationLayers.count(layer) > 0)
			{
				validationLayers.push_back(layer.c_str());
			}
			else
			{
				std::cout << "Warning: Missing requested validation layer: " << layer << std::endl;
			}
		}

		std::vector<const char*> instanceExtensions;
		instanceExtensions.reserve(settings.instanceExtensions.size());

		for (const auto& extension : settings.instanceExtensions)
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
		return canPhysicalDevicePresent(device);
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
		auto allDevices = getPhysicalDevices();

		if (settings.physicalDeviceOverride)
		{
			VkPhysicalDevice deviceOverride = nullptr;

			for (const auto& device : allDevices)
			{
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(device, &properties);

				if (PhysicalDeviceID(properties) == settings.physicalDeviceOverride)
				{
					deviceOverride = device;
					break;
				}
			}

			if (deviceOverride == nullptr)
			{
				std::cout << "Warning requested physical device not found." << std::endl;
			}
			else
			{
				if (isPhysicalDeviceSuitable(deviceOverride, settings))
				{
					VkPhysicalDeviceProperties deviceProperties;
					vkGetPhysicalDeviceProperties(deviceOverride, &deviceProperties);

					m_physicalDevice = deviceOverride;
					std::cout << "Using GPU: " << deviceProperties.deviceName << std::endl;
					return;
				}
				else
				{
					std::cout << "Warning requested physical device not suitable." << std::endl;
				}
			}
		}

		auto suitableDevices = getSuitablePhysicalDevices(m_vulkanInstance, settings);

		// Try to find a discrete gpu
		for (const auto& device : suitableDevices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

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
		if (suitableDevices.size() > 0)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(suitableDevices[0], &deviceProperties);

			std::cout << "Using GPU: " << deviceProperties.deviceName << std::endl;
			m_physicalDevice = suitableDevices[0];
			return;
		}

		throw std::runtime_error("No suitable physical device found.");
	}

	int VulkanInstance::getBestQueueFamily()
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

		// Find the first graphics queue:
		for (int i = 0; i < queueFamilyCount; ++i)
		{
			auto queueFlags = queueFamilies[i].queueFlags;

			if (queueFlags & VK_QUEUE_GRAPHICS_BIT &&
				queueFlags & VK_QUEUE_COMPUTE_BIT &&
				queueFlags & VK_QUEUE_TRANSFER_BIT &&
				queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
			{
				return i;
			}
		}
		throw std::runtime_error("Failed to find Vulkan graphics queue.");
	}

	void VulkanInstance::createLogicalDevice(const VulkanSettings& settings)
	{
		const auto availableValidationLayers = getAvailableValidationLayerNames();

		std::vector<const char*> validationLayers;
		validationLayers.reserve(settings.validationLayers.size());

		for (const auto& layer : settings.validationLayers)
		{
			if (availableValidationLayers.count(layer) > 0)
			{
				validationLayers.push_back(layer.c_str());
			}
		}

		std::vector<const char*> deviceExtensions;
		deviceExtensions.reserve(settings.deviceExtensions.size());

		for (const auto& extension : settings.deviceExtensions)
		{
			deviceExtensions.push_back(extension.c_str());
		}

		const float mainQueuePriority = 1.0f;
		const int mainQueueIndex = getBestQueueFamily();

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.queueFamilyIndex = mainQueueIndex;
		queueCreateInfo.pQueuePriorities = &mainQueuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
		createInfo.enabledExtensionCount = deviceExtensions.size();
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		VkResult result = vkCreateDevice(m_physicalDevice , &createInfo, nullptr, &m_logicalDevice);

		if (result == VK_SUCCESS)
		{
			m_destructLogicalDevice = [this]()
			{
				std::cout << "Destroying logical device." << std::endl;
				vkDestroyDevice(m_logicalDevice, nullptr);
			};

			vkGetDeviceQueue(m_logicalDevice, mainQueueIndex, 0, &m_mainCommandQueue);
		}
		else
		{
			throw std::runtime_error("Failed to create logical device.");
		}
	}

}
