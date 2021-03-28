#pragma once

#include <vulkan/vulkan.h>



namespace Vulkan
{
	class PhysicalDevice;

	class QueueFamily
	{
	public:

		QueueFamily();
		QueueFamily(const PhysicalDevice&, int index);


	private:


	};

}
