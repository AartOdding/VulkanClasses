#pragma once

#include <vulkan/vulkan.h>

#include <Vk/PhysicalDevice.hpp>


namespace Vulkan
{

	class QueueFamily
	{
	public:

		QueueFamily();
		QueueFamily(const PhysicalDevice&, int index);


	private:


	};

}
