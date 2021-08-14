#pragma once

#include <stdint.h>
#include <vulkan/vulkan.h>


namespace Vulkan::Utils
{

	struct Version
	{
		const uint32_t major = 0;
		const uint32_t minor = 0;
		const uint32_t patch = 0;
	};

	constexpr uint32_t getVulkanVersion(const Version& version)
	{
		return VK_MAKE_VERSION(version.major, version.minor, version.patch);
	}

}
