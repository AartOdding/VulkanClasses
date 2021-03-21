#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "Utils/DestructCall.hpp"
#include "Vulkan/VulkanSettings.hpp"


namespace Vulkan
{

	class VulkanInstance
	{
	public:

		VulkanInstance(const VulkanSettings& settings);
		~VulkanInstance();

		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance(VulkanInstance&&) = delete;

		static std::unordered_set<std::string> getAvailableValidationLayers();
		static std::unordered_set<std::string> getAvailableInstanceExtensions();

	private:

		static VulkanSettings addDefaultExtensionsAndValidationLayers(const VulkanSettings& settings);

		void initGlfw();
		void initVulkan(const VulkanSettings& settings);

		bool isPhysicalDeviceSuitable(VkPhysicalDevice device, const VulkanSettings& settings) const;
		std::vector<VkPhysicalDevice> getSuitablePhysicalDevices(VkInstance instance, const VulkanSettings& settings) const;

		void pickPhysicalDevice(const VulkanSettings& settings);
		void findQueueFamilies(int& graphicsQueue);
		void createLogicalDevice();


		Utils::DestructCall m_destructGlfw;
		Utils::DestructCall m_destructVulkan;

		VkInstance m_vulkanInstance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_logicalDevice;
	};

}
