#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include <Vulkan/SwapChain.hpp>
#include <Vulkan/WindowSurface.hpp>
#include <Vulkan/LogicalDevice.hpp>


namespace
{

	std::vector<std::uint32_t> getUniqueQueueFamilies(const Vulkan::LogicalDevice* logicalDevice)
	{
		const auto& queues = logicalDevice->queueFamilies();
		std::set<std::uint32_t> queueIndicesSet;

		for (const auto& [k, v] : queues)
		{
			queueIndicesSet.insert(v);
		}

		return { queueIndicesSet.begin(), queueIndicesSet.end() };
	}


	VkExtent2D getSurfaceExtent(const VkSurfaceCapabilitiesKHR& capabilities, const Vulkan::WindowSurface* windowSurface)
	{
		VkExtent2D extent{};

		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			extent = capabilities.currentExtent;
		}
		else
		{
			auto [width, height] = windowSurface->frameBufferSize();
			extent.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			extent.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		}

		return extent;
	}

}


namespace Vulkan
{

	std::set<VkSurfaceFormatKHR> SwapChain::supportedFormats(const LogicalDevice& logicalDevice, const WindowSurface& windowSurface)
	{
		std::vector<VkSurfaceFormatKHR> formats;

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(logicalDevice.physicalDevice(), windowSurface.surface(), &formatCount, nullptr);

		if (formatCount > 0)
		{
			formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(logicalDevice.physicalDevice(), windowSurface.surface(), &formatCount, formats.data());
		}
		return std::set<VkSurfaceFormatKHR>(formats.begin(), formats.end());
	}

	SwapChain::SwapChain(const LogicalDevice* logicalDevice, const WindowSurface* windowSurface, SwapChainSettings settings)
		: m_logicalDevice(logicalDevice)
		, m_windowSurface(windowSurface)
		, m_createInfo{}
	{
		if (!m_logicalDevice)
		{
			throw std::runtime_error("Cannot create swapchain without logical device.");
		}
		if (!m_windowSurface)
		{
			throw std::runtime_error("Cannot create swapchain without window surface.");
		}

		VkSurfaceCapabilitiesKHR capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_logicalDevice->physicalDevice(), m_windowSurface->surface(), &capabilities);

		// =====================

		m_createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		m_createInfo.surface = m_windowSurface->surface();

		m_createInfo.minImageCount = settings.imageCount;
		m_createInfo.imageFormat = settings.surfaceFormat.format;
		m_createInfo.imageColorSpace = settings.surfaceFormat.colorSpace;
		m_createInfo.imageExtent = getSurfaceExtent(capabilities, m_windowSurface);
		m_createInfo.imageArrayLayers = 1;
		m_createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		auto queueFamilies = getUniqueQueueFamilies(m_logicalDevice);

		if (queueFamilies.size() == 1)
		{
			m_createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			m_createInfo.queueFamilyIndexCount = 0;
			m_createInfo.pQueueFamilyIndices = nullptr;
		}
		else
		{
			m_createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			m_createInfo.queueFamilyIndexCount = queueFamilies.size();
			m_createInfo.pQueueFamilyIndices = queueFamilies.data();
		}

		m_createInfo.preTransform = capabilities.currentTransform;
		m_createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		m_createInfo.presentMode = settings.presentMode;
		m_createInfo.clipped = VK_TRUE;
		
		m_createInfo.oldSwapchain = VK_NULL_HANDLE;

		// ======================

		if (vkCreateSwapchainKHR(m_logicalDevice->get(), &m_createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create swap chain.");
		}
		else
		{
			m_swapChainCleanup = [logicalDevice = m_logicalDevice->get(), swapChain = m_swapChain]()
			{
				std::cout << "Destroying Swap Chain." << std::endl;
				vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
			};
		}

		// ======================

		std::uint32_t imageCount;
		vkGetSwapchainImagesKHR(m_logicalDevice->get(), m_swapChain, &imageCount, nullptr);
		m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_logicalDevice->get(), m_swapChain, &imageCount, m_swapChainImages.data());

		m_swapChainImageViews.resize(imageCount);
		m_imageViewCleanups.resize(imageCount);

		for (size_t i = 0; i < m_swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_swapChainImages[i];

			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = settings.surfaceFormat.format;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_logicalDevice->get(), &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create image views!");
			}
			else
			{
				m_imageViewCleanups[i] = [logicalDevice = m_logicalDevice->get(), imageView = m_swapChainImageViews[i], i]()
				{
					std::cout << "Destroying Swap Chain Image View " << i << std::endl;
					vkDestroyImageView(logicalDevice, imageView, nullptr);
				};
			}
		}
	}

	VkSwapchainKHR SwapChain::get() const
	{
		return m_swapChain;
	}

	VkSurfaceFormatKHR SwapChain::surfaceFormat() const
	{
		return { m_createInfo.imageFormat, m_createInfo.imageColorSpace };
	}

	VkPresentModeKHR SwapChain::presentMode() const
	{
		return m_createInfo.presentMode;
	}

}
