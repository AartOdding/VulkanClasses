#include <iostream>
#include <stdexcept>

#include <Vulkan/Utils/SetOperations.hpp>
#include <Vulkan/Instance.hpp>
#include <Vulkan/LogicalDevice.hpp>


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
		: m_physicalDevice(settings.physicalDevice)
		, m_enabledDeviceFeatures(settings.deviceFeatures)
	{
		if (instance == nullptr)
		{
			throw std::runtime_error("Cannot create logical device without instance.");
		}
		if (settings.physicalDevice == nullptr)
		{
			throw std::runtime_error("Cannot create logical device without physical device.");
		}

		std::unordered_map<int, QueueFamilyTemp> queueFamilies;

		// Sort all queues by family
		for (const auto& [name, queueSettings] : settings.queuesToCreate)
		{
			queueFamilies[queueSettings.queueFamily].queues.push_back(
				{ name, queueSettings.queueFamily, queueSettings.priority });

			m_queueFamilies[name] = queueSettings.queueFamily;
			m_queuePriorities[name] = queueSettings.priority;
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
				static_cast<uint32_t>(index),
				static_cast<uint32_t>(family.queues.size()),
				family.priorities.data()
			});
		}

		const Vulkan::PhysicalDevice physicalDevice{ settings.physicalDevice };

		// Collect device extensions:
		const auto availableExtensions = physicalDevice.availableDeviceExtensions();
		const auto desiredExtensions = Utils::setUnion(settings.requiredDeviceExtensions, settings.optionalDeviceExtensions);
		const auto missingExtensions = Utils::setDifference(desiredExtensions, availableExtensions);

		for (const auto& missingExtension : missingExtensions)
		{
			if (settings.requiredDeviceExtensions.count(missingExtension))
			{
				throw std::runtime_error("Missing required instance extension: " + missingExtension);
			}
			else
			{
				std::cerr << "[Warning] Missing optional instance extension: " << missingExtension << std::endl;
			}
		}

		m_enabledDeviceExtensionNames = Utils::setIntersection(desiredExtensions, availableExtensions);
		const auto extensions = Utils::getCStringList(m_enabledDeviceExtensionNames);

		// Create info struct
		VkDeviceCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;

		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = queueCreateInfos.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledLayerNames = nullptr;
		createInfo.enabledLayerCount = 0;
		createInfo.pEnabledFeatures = &settings.deviceFeatures;

		// Do the actual creation
		if (vkCreateDevice(settings.physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create logical device!");
		}

		// Retrieve queue handles
		for (const auto& [index, queueFamily] : queueFamilies)
		{
			for (int i = 0; i < queueFamily.queues.size(); ++i)
			{
				VkQueue queue = nullptr;
				vkGetDeviceQueue(m_logicalDevice, index, i, &queue);
				m_queues[queueFamily.queues[i].name] = queue;
			}
		}

	}

	LogicalDevice::~LogicalDevice()
	{
		std::cout << "Destroying Logical Device." << std::endl;
		vkDestroyDevice(m_logicalDevice, nullptr);
	}

	VkDevice LogicalDevice::get() const
	{
		return m_logicalDevice;
	}

	VkPhysicalDevice LogicalDevice::physicalDevice() const
	{
		return m_physicalDevice;
	}

	const std::unordered_map<std::string, VkQueue>& LogicalDevice::queues() const
	{
		return m_queues;
	}

	const std::unordered_map<std::string, int>& LogicalDevice::queueFamilies() const
	{
		return m_queueFamilies;
	}

	const std::unordered_map<std::string, float>& LogicalDevice::queuePriorities() const
	{
		return m_queuePriorities;
	}

	const std::set<std::string>& LogicalDevice::enabledDeviceExtensionNames() const
	{
		return m_enabledDeviceExtensionNames;
	}

	bool LogicalDevice::isDeviceExtensionEnabled(const std::string& deviceExtensionName) const
	{
		return m_enabledDeviceExtensionNames.count(deviceExtensionName);
	}

	const VkPhysicalDeviceFeatures& LogicalDevice::enabledDeviceFeatures() const
	{
		return m_enabledDeviceFeatures;
	}

}
