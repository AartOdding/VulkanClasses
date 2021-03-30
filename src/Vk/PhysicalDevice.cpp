#include <Vk/PhysicalDevice.hpp>


namespace Vulkan
{

	PhysicalDevice::PhysicalDevice()
		: m_device(nullptr)
	{

	}

	PhysicalDevice::PhysicalDevice(VkPhysicalDevice device)
		: m_device(device)
	{

	}

	VkPhysicalDevice PhysicalDevice::get() const
	{
		return m_device;
	}

	std::vector<VkQueueFamilyProperties> PhysicalDevice::availableQueueFamilyProperties() const
	{
		std::vector<VkQueueFamilyProperties> queueFamilies;

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_device, &count, nullptr);

		if (count > 0)
		{
			queueFamilies.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties(m_device, &count, queueFamilies.data());
		}
		return queueFamilies;
	}

	std::vector<VkExtensionProperties> PhysicalDevice::availableDeviceExtensionProperties() const
	{
		std::vector<VkExtensionProperties> properties;

		if (m_device)
		{
			uint32_t count = 0;
			vkEnumerateDeviceExtensionProperties(m_device, nullptr, &count, nullptr);

			if (count > 0)
			{
				properties.resize(count);
				vkEnumerateDeviceExtensionProperties(m_device, nullptr, &count, properties.data());
			}
		}

		return properties;
	}

	std::set<std::string> PhysicalDevice::availableDeviceExtensionNames() const
	{
		const auto properties = availableDeviceExtensionProperties();

		std::set<std::string> names;

		for (const auto& extensionProperties : properties)
		{
			names.insert(extensionProperties.extensionName);
		}
		return names;
	}

}
