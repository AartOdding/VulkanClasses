#include <Vulkan/PhysicalDevice.hpp>
#include <Vulkan/QueueFamily.hpp>


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

	static std::vector<VkQueueFamilyProperties2> availableQueueFamilyProperties(VkPhysicalDevice device)
	{
		std::vector<VkQueueFamilyProperties2> queueFamilies;

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties2(device, &count, nullptr);

		if (count > 0)
		{
			queueFamilies.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties2(device, &count, queueFamilies.data());
		}
		return queueFamilies;
	}

	std::vector<QueueFamily> PhysicalDevice::queueFamilies() const
	{
		const auto properties = availableQueueFamilyProperties(m_device);
		std::vector<QueueFamily> queue_families;
		queue_families.reserve(properties.size());

		for (size_t i = 0; i < properties.size(); ++i)
		{
			queue_families.emplace_back(m_device, i, properties[i]);
		}

		return queue_families;
	}

	static std::vector<VkExtensionProperties> availableDeviceExtensionProperties(VkPhysicalDevice device)
	{
		std::vector<VkExtensionProperties> properties;

		if (device)
		{
			uint32_t count = 0;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

			if (count > 0)
			{
				properties.resize(count);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &count, properties.data());
			}
		}

		return properties;
	}

	std::set<std::string> PhysicalDevice::availableDeviceExtensions() const
	{
		const auto properties = availableDeviceExtensionProperties(m_device);

		std::set<std::string> names;

		for (const auto& extensionProperties : properties)
		{
			names.insert(extensionProperties.extensionName);
		}
		return names;
	}

	PhysicalDevice::operator VkPhysicalDevice() const
	{
		return m_device;
	}

}
