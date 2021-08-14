#include <algorithm>
#include <bitset>
#include <cassert>

#include <Vulkan/PhysicalDevice.hpp>
#include <Vulkan/QueueFamily.hpp>

namespace Vulkan
{
	namespace
	{

		struct QueueFamily
		{
			VkPhysicalDevice physical_device = nullptr;
			uint32_t queue_family_index = 0;
			VkQueueFamilyProperties properties{};
			uint32_t count_remaining = 0;
		};

		bool operator==(const QueueFamily& lhs, const QueueFamily& rhs)
		{
			return lhs.physical_device == rhs.physical_device && lhs.queue_family_index == rhs.queue_family_index;
		}

		std::vector<QueueFamily> getQueueFamilies(VkPhysicalDevice physical_device)
		{
			std::vector<VkQueueFamilyProperties> queueFamilyProperties;

			uint32_t count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);

			if (count > 0)
			{
				queueFamilyProperties.resize(count);
				vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, queueFamilyProperties.data());
			}

			std::vector<QueueFamily> queueFamilies(queueFamilyProperties.size());

			for (size_t i = 0; i < queueFamilies.size(); ++i)
			{
				queueFamilies[i].physical_device = physical_device;
				queueFamilies[i].properties = queueFamilyProperties[i];
				queueFamilies[i].queue_family_index = i;
				queueFamilies[i].count_remaining = queueFamilyProperties[i].queueCount;
			}

			return queueFamilies;
		}

		template<typename Container, typename Predicate>
		void remove_if(Container& container, const Predicate& predicate)
		{
			container.erase(std::remove_if(container.begin(), container.end(), predicate), container.end());
		}

		uint32_t bit_count(uint32_t value)
		{
			return std::bitset<32>(value).count();
		}

		int findBestQueue(VkPhysicalDevice device, const std::vector<QueueFamily>& families, const QueueRequirements& requirements)
		{
			// require still available
			// require flags
			// require present support
			// require granularity

			// find queue with least extra flags
			// sort by better granularity?
			// lowest index
			// find queue with lowest initial allowed count

			auto filtered = families;

			// Require that the queue family has atleast one instantiation left.
			remove_if(filtered, [](const QueueFamily& family) { return family.count_remaining <= 0; });

			// Filter out queue families that don't support all the required queue flags.
			remove_if(filtered, [&requirements](const QueueFamily& family) { return (family.properties.queueFlags & requirements.queueFlags) != requirements.queueFlags; });

			if (requirements.presentTo)
			{
				// If presentation is required filter out all queue families that can't present to the surface.
				remove_if(filtered, [&requirements, device](const QueueFamily& family)
				{
					VkBool32 supported = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(device, family.queue_family_index, requirements.presentTo, &supported);
					return supported;
				});
			}

			if (requirements.minImageTransferGranularity)
			{
				assert(false); // TODO implement
			}

			// return true if a should go before b
			std::sort(filtered.begin(), filtered.end(), [](const QueueFamily& a, const QueueFamily& b)
			{
				return bit_count(a.properties.queueFlags) < bit_count(b.properties.queueFlags);
			});

			return filtered.empty() ? -1 : filtered[0].queue_family_index;
		}


	}
}



namespace Vulkan
{

	PhysicalDevice::PhysicalDevice()
		: m_device(nullptr)
	{

	}

	PhysicalDevice::PhysicalDevice(VkPhysicalDevice device)
		: m_device(device)
	{

	}

	bool PhysicalDevice::supportsQueues(const std::vector<QueueRequirements>& requirements) const
	{
		return resolveQueues(requirements).second;
	}

	std::pair<std::vector<uint64_t>, bool> PhysicalDevice::resolveQueues(const std::vector<QueueRequirements>& requirements) const
	{
		auto families = getQueueFamilies(m_device);
		std::vector<uint64_t> matches;

		for (auto& req : requirements)
		{
			auto best = findBestQueue(m_device, families, req);

			if (best != -1)
			{
				families[best].count_remaining--;
				matches.push_back(best);
			}
			else
			{
				return { {}, false };
			}
		}

		return { matches, true };
	}


	static std::vector<VkQueueFamilyProperties2> availableQueueFamilyProperties(VkPhysicalDevice device)
	{
		std::vector<VkQueueFamilyProperties2> queueFamilies;

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties2(device, &count, nullptr);

		if (count > 0)
		{
			queueFamilies.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties2(device, &count, queueFamilies.data());
		}
		return queueFamilies;
	}
	/*
	std::vector<QueueFamily> PhysicalDevice::queueFamilies() const
	{
		const auto properties = availableQueueFamilyProperties(m_device);
		std::vector<QueueFamily> queue_families;
		queue_families.reserve(properties.size());

		for (size_t i = 0; i < properties.size(); ++i)
		{
			queue_families.emplace_back(m_device, i, properties[i]);
		}

		return queue_families;
	}*/

	static std::vector<VkExtensionProperties> availableDeviceExtensionProperties(VkPhysicalDevice device)
	{
		std::vector<VkExtensionProperties> properties;

		if (device)
		{
			uint32_t count = 0;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

			if (count > 0)
			{
				properties.resize(count);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &count, properties.data());
			}
		}

		return properties;
	}

	std::set<std::string> PhysicalDevice::availableDeviceExtensions() const
	{
		const auto properties = availableDeviceExtensionProperties(m_device);

		std::set<std::string> names;

		for (const auto& extensionProperties : properties)
		{
			names.insert(extensionProperties.extensionName);
		}
		return names;
	}

	PhysicalDevice::operator VkPhysicalDevice() const
	{
		return m_device;
	}

}
