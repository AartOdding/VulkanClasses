#pragma once

#include <vector>

#include <vulkan/vulkan.h>


namespace Vulkan
{
	class PhysicalDeviceID
	{
	public:

		PhysicalDeviceID() = default;
		PhysicalDeviceID(VkPhysicalDeviceProperties device);

		const std::vector<uint8_t> value;

		explicit operator bool() const;

		bool operator==(const PhysicalDeviceID& rhs);

	};

}
