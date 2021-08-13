#include <iostream>
#include <stdexcept>

#include <Vulkan/PipelineLayout.hpp>
#include <Vulkan/LogicalDevice.hpp>



namespace Vulkan
{


	PipelineLayout::PipelineLayout(const LogicalDevice* logicalDevice)
	{
		if (!logicalDevice)
		{
			throw std::runtime_error("Cannot create pipeline layout without logical device.");
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(logicalDevice->get(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout.");
		}
		else
		{
			m_pipelineLayoutCleanup = [device = logicalDevice->get(), pipelineLayout = m_pipelineLayout]()
			{
				std::cout << "Destroying Pipeline Layout." << std::endl;
				vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
			};
		}
	}

	VkPipelineLayout PipelineLayout::get() const
	{
		return m_pipelineLayout;
	}

}
