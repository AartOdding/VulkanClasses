#pragma once

#include <stdint.h>

#include <vulkan/vulkan.h>

#include <Vulkan/Utils/NoCopy.hpp>
#include <Vulkan/Utils/ScopeGuard.hpp>
#include <Vulkan/PipelineLayout.hpp>
#include <Vulkan/ShaderStage.hpp>



namespace Vulkan
{
	class LogicalDevice;


	class GraphicsPipeline : Utils::NoCopy
	{
	public:

		GraphicsPipeline(const LogicalDevice* logicalDevice, uint32_t w, uint32_t h);

	private:

		VkPipeline m_pipeline;

		Utils::ScopeGuard m_graphicsPipelineCleanup;

		ShaderStage m_vertexShader;
		ShaderStage m_fragmentShader;
		PipelineLayout m_pipelineLayout;

	};

}
