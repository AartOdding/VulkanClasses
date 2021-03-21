#pragma once

#include <Vulkan/vulkan.h>


namespace Utils
{
	struct Version
	{
		const int major = 0;
		const int minor = 0;
		const int patch = 0;

		uint32_t vkVersion() const
		{
			return VK_MAKE_VERSION(major, minor, patch);
		}
	};

}
