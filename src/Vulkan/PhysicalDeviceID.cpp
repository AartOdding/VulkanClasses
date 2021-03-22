#include "Vulkan/PhysicalDeviceID.hpp"

namespace Vulkan
{
	PhysicalDeviceID::PhysicalDeviceID(VkPhysicalDeviceProperties device)
		: value(&device.pipelineCacheUUID[0], &device.pipelineCacheUUID[0] + VK_UUID_SIZE)
	{

	}

	PhysicalDeviceID::operator bool() const
	{
		return value.size() == VK_UUID_SIZE;
	}

	bool PhysicalDeviceID::operator==(const PhysicalDeviceID& rhs)
	{
		return value == rhs.value;
	}
}
