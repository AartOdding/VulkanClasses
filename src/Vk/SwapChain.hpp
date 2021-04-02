#pragma once

#include <vector>
#include <set>

#include <Utils/NoCopy.hpp>
#include <Utils/ScopeGuard.hpp>


inline bool operator<(const VkSurfaceFormatKHR& lhs, const VkSurfaceFormatKHR& rhs)
{
	if (lhs.colorSpace != rhs.colorSpace)
	{
		return lhs.colorSpace < rhs.colorSpace;
	}
	else
	{
		return lhs.format > rhs.format;
	}
}


namespace Vulkan
{
	class LogicalDevice;
	class WindowSurface;

	struct SwapChainSettings
	{
		VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		VkCompositeAlphaFlagBitsKHR alphaFlags = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		int imageCount = 2;
	};


	class SwapChain : Utils::NoCopy
	{
	public:

		static std::set<VkSurfaceFormatKHR> supportedFormats(const LogicalDevice& logicalDevice, const WindowSurface& windowSurface);

		SwapChain(const LogicalDevice* logicalDevice, const WindowSurface* windowSurface, SwapChainSettings settings = SwapChainSettings());

	private:

		VkSwapchainKHR m_swapChain;
		VkSwapchainCreateInfoKHR m_createInfo;

		const LogicalDevice* m_logicalDevice;
		const WindowSurface* m_windowSurface;

		std::vector<VkImage> m_swapChainImages;
		std::vector<VkImageView> m_swapChainImageViews;

		Utils::ScopeGuard m_swapChainCleanup;
		std::vector<Utils::ScopeGuard> m_imageViewCleanups;
	};

}
