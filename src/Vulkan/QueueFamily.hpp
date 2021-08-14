#pragma once

#include <optional>
#include <set>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <Vulkan/Forward.hpp>
#include <Vulkan/Utils/Macros.hpp>



namespace Vulkan
{

	struct QueueRequirements
	{
		VkQueueFlags queueFlags = 0;
		VkSurfaceKHR presentTo = nullptr;
		std::optional<VkExtent3D> minImageTransferGranularity;
	};

	/*
	namespace DefaultQueues
	{
		static constexpr QueueRequirements Graphics{ true, true, true, false };
		static constexpr QueueRequirements DedicatedTransfer{ false, false, true, false };
		static constexpr QueueRequirements DedicatedCompute{ false, true, true, false };
	}*/

	/*
	class QueueFamily
	{
	public:

		QueueFamily();
		QueueFamily(const VkPhysicalDevice& physical_device, uint32_t index, VkQueueFamilyProperties2 properties);

		VkPhysicalDevice physicalDevice() const;

		uint32_t index() const;

		const VkQueueFamilyProperties& properties() const;
		const VkQueueFamilyProperties2& properties2() const;

		VULKAN_CLASSES_DEFAULT_COPY_AND_MOVE(QueueFamily)

	private:

		VkPhysicalDevice m_physical_device;
		uint32_t m_index;
		VkQueueFamilyProperties2 m_properties;

	};
	*/
}
