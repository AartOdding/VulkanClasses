#include <Vk/RenderPass.hpp>
#include <Vk/SwapChain.hpp>


namespace Vulkan
{

	RenderPass::RenderPass(const LogicalDevice* logicalDevice, const SwapChain* swapChain)
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapChain->surfaceFormat().format;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	}

	VkRenderPass RenderPass::get() const
	{

	}

}
