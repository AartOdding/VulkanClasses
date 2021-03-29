#include <Vk/PhysicalDevice.hpp>


namespace Vulkan
{

	PhysicalDevice::PhysicalDevice()
		: m_device(nullptr)
		, m_instance(nullptr)
	{

	}

	PhysicalDevice::PhysicalDevice(const Instance* vulkanInstance, VkPhysicalDevice device)
		: m_device(device)
		, m_instance(vulkanInstance)
	{

	}

	VkPhysicalDevice PhysicalDevice::get() const
	{
		return m_device;
	}

	const Instance* PhysicalDevice::vulkanInstance() const
	{
		return m_instance;
	}

	std::vector<QueueFamily> PhysicalDevice::availableQueueFamilies()
	{
		std::vector<QueueFamily> queueFamilies;

		unsigned int count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_device, &count, nullptr);

		queueFamilies.reserve(count);
		for (int i = 0; i < count; ++i)
		{
			queueFamilies.emplace_back(this, i);
		}

		return queueFamilies;
	}

}
