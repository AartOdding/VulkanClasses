#include <type_traits>

#include <Vk/QueueFamily.hpp>

namespace Vulkan
{

	QueueFlags operator|(QueueFlags lhs, QueueFlags rhs)
	{
		return static_cast<QueueFlags>(
			static_cast<std::underlying_type<QueueFlags>::type>(lhs) |
			static_cast<std::underlying_type<QueueFlags>::type>(rhs));
	}

	QueueFlags operator&(QueueFlags lhs, QueueFlags rhs)
	{
		return static_cast<QueueFlags>(
			static_cast<std::underlying_type<QueueFlags>::type>(lhs) &
			static_cast<std::underlying_type<QueueFlags>::type>(rhs));
	}

	QueueFamily::QueueFamily()
		: m_device(nullptr)
		, m_index(-1)
	{

	}

	QueueFamily::QueueFamily(const PhysicalDevice* device, int index)
		: m_device(device)
		, m_index(index)
	{

	}

	const PhysicalDevice* QueueFamily::physicalDevice() const
	{
		return m_device;
	}

	int QueueFamily::index() const
	{
		return m_index;
	}

}
