#include <Vulkan/QueueFamily.hpp>


namespace Vulkan
{

	QueueFamily::QueueFamily()
		: m_physical_device()
		, m_index()
		, m_properties()
	{

	}

	QueueFamily::QueueFamily(const VkPhysicalDevice& physical_device, uint32_t index, VkQueueFamilyProperties2 properties)
		: m_physical_device(physical_device)
		, m_index(index)
		, m_properties(properties)
	{
	}

	VkPhysicalDevice QueueFamily::physicalDevice() const
	{
		return m_physical_device;
	}

	uint32_t QueueFamily::index() const
	{
		return m_index;
	}

	const VkQueueFamilyProperties& QueueFamily::properties() const
	{
		return m_properties.queueFamilyProperties;
	}

	const VkQueueFamilyProperties2& QueueFamily::properties2() const
	{
		return m_properties;
	}

}
