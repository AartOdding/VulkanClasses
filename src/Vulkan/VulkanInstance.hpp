#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "Utils/DestructCall.hpp"
#include "Vulkan/VulkanSettings.hpp"
#include "Vulkan/VulkanWindow.hpp"


namespace Vulkan
{

	class VulkanInstance
	{
	public:

		VulkanInstance(const VulkanSettings& settings);
		~VulkanInstance();

		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance(VulkanInstance&&) = delete;

		static std::vector<VkLayerProperties> getAvailableValidationLayers();
		static std::vector<VkExtensionProperties> getAvailableInstanceExtensions();

		static std::unordered_set<std::string> getAvailableValidationLayerNames();
		static std::unordered_set<std::string> getAvailableInstanceExtensionNames();

		std::vector<VkPhysicalDevice> getPhysicalDevices() const;
		std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device) const;

		VkPhysicalDevice getActivePhysicalDevice();
		VkQueue getMainCommandQueue();

	private:

		static VulkanSettings completeSettings(const VulkanSettings& settings);
		static bool comparePhysicalDevices(const VkPhysicalDevice& lhs, const VkPhysicalDevice& rhs);

		void initGlfw();
		void createVulkanInstance(const VulkanSettings& settings);

		bool canPhysicalDevicePresent(VkPhysicalDevice device) const;
		bool isPhysicalDeviceSuitable(VkPhysicalDevice device, const VulkanSettings& settings) const;
		std::vector<VkPhysicalDevice> getSuitablePhysicalDevices(VkInstance instance, const VulkanSettings& settings) const;

		void pickPhysicalDevice(const VulkanSettings& settings);
		int getBestQueueFamilyIndex();
		void createLogicalDevice(const VulkanSettings& settings);

		Utils::DestructCall m_destructGlfw;
		Utils::DestructCall m_destructVulkan;
		Utils::DestructCall m_destructLogicalDevice;

		VkInstance m_vulkanInstance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_logicalDevice;
		VkQueue m_mainCommandQueue;
	};

}
