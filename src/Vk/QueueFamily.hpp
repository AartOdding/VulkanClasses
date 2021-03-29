#pragma once

#include <vulkan/vulkan.h>



namespace Vulkan
{
	class PhysicalDevice;


	enum class QueueFlags
	{
		None          = 0,
		Graphics      = 1 << 0,
		Compute       = 1 << 1,
		Transfer      = 1 << 2,
		SparseBinding = 1 << 3
	};

	QueueFlags operator|(QueueFlags lhs, QueueFlags rhs);
	QueueFlags operator&(QueueFlags lhs, QueueFlags rhs);


	class QueueFamily
	{
	public:

		QueueFamily();
		QueueFamily(const PhysicalDevice* device, int index);

		const PhysicalDevice* physicalDevice() const;
		int index() const;

	private:

		const PhysicalDevice* m_device;
		int m_index;

	};

}
