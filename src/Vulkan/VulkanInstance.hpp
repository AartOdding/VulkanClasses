#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "Utils/NoCopy.hpp"
#include "Utils/ScopeGuard.hpp"
#include "Vulkan/VulkanSettings.hpp"
#include "Vulkan/VulkanWindow.hpp"


namespace Vulkan
{

	class VulkanInstance : Utils::NoCopy
	{
	public:

		VulkanInstance(const VulkanSettings& settings);
		~VulkanInstance();

		VkPhysicalDevice getActivePhysicalDevice();
		VkQueue getMainCommandQueue();

		VkInstance getVkInstance();
		const VkInstance getVkInstance() const;

	private:

		static VulkanSettings completeSettings(const VulkanSettings& settings);
		
		void initGlfw();
		void createVulkanInstance(const VulkanSettings& settings);

		bool canPhysicalDevicePresent(VkPhysicalDevice device) const;
		bool isPhysicalDeviceSuitable(VkPhysicalDevice device, const VulkanSettings& settings) const;
		std::vector<VkPhysicalDevice> getSuitablePhysicalDevices(VkInstance instance, const VulkanSettings& settings) const;

		void pickPhysicalDevice(const VulkanSettings& settings);
		void createLogicalDevice(const VulkanSettings& settings);
		int getBestQueueFamily();

		Utils::ScopeGuard m_destructGlfw;
		Utils::ScopeGuard m_destructVulkan;
		Utils::ScopeGuard m_destructLogicalDevice;

		VkInstance m_vulkanInstance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_logicalDevice;
		VkQueue m_mainCommandQueue;
	};

}
