#include <Vk/Instance.hpp>


namespace Vulkan
{

	Instance(const InstanceSettings& settings)
		: m_settings(settings)
	{
		// GLFW part:

		if (!settings.headless)
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

		// Vulkan part:

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

	VkInstance get() const
	{
		return m_instance;
	}

	const InstanceSettings& settings() const
	{
		return m_settings;
	}

	std::vector<PhysicalDevice> availablePhysicalDevices() const
	{
		std::vector<VkPhysicalDevice> devices;

		uint32_t count = 0;
		vkEnumeratePhysicalDevices(m_instance, &count, nullptr);

		if (count > 0)
		{
			availablePhysicalDevices.resize(count);
			vkEnumeratePhysicalDevices(instance, &count, devices.data());
		}
		return devices;
	}

}
