#pragma once

#include <vulkan/vulkan.h>

#include <Utils/NoCopy.hpp>
#include <Utils/ScopeGuard.hpp>
#include <Vk/PipelineLayout.hpp>
#include <Vk/ShaderStage.hpp>



namespace Vulkan
{
	class LogicalDevice;
	class SwapChain;


	class RenderPass : Utils::NoCopy
	{
	public:

		RenderPass(const LogicalDevice* logicalDevice, const SwapChain * swapChain);

		VkRenderPass get() const;

	private:

		VkRenderPass m_renderPass;

		Utils::ScopeGuard m_renderPassCleanup;

	};

}
