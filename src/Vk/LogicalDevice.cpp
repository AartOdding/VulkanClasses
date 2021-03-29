#include <Vk/LogicalDevice.hpp>

namespace
{

	struct QueueTemp
	{
		std::string name;
		int queueFamily = 0;
		float priority = 1.0f;
	};

	struct QueueFamilyTemp
	{
		std::vector<float> priorities;
		std::vector<QueueTemp> queues;
	};
}


namespace Vulkan
{

	LogicalDevice::LogicalDevice(const Instance* instance, LogicalDeviceSettings settings)
	{
		std::unordered_map<int, QueueFamilyTemp> queueFamilies;

		// Sort all queues by family
		for (const auto& [name, queueSettings] : settings.queuesToCreate)
		{
			queueFamilies[queueSettings.queueFamily].queues.push_back(
				{ name, queueSettings.queueFamily, queueSettings.priority });
		}

		// For each family create priories for each queue to be instantiated
		for (auto& [index, family] : queueFamilies)
		{
			family.priorities.reserve(family.queues.size());

			for (const auto& queue : family.queues)
			{
				family.priorities.push_back(queue.priority);
			}
		}

		// Create the vector of VkDeviceQueueCreateInfo:
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		queueCreateInfos.reserve(queueFamilies.size());

		for (const auto& [index, family] : queueFamilies)
		{
			queueCreateInfos.push_back(
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				0,
				index,
				family.queues.size(),
				family.priorities.data()
			});
		}

		VkDeviceCreateInfo info;
	}

	LogicalDevice::~LogicalDevice()
	{

	}

}
