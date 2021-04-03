#pragma once

#include <vulkan/vulkan.h>

#include <Utils/NoCopy.hpp>
#include <Utils/ScopeGuard.hpp>
#include <Vk/PipelineLayout.hpp>
#include <Vk/ShaderStage.hpp>



namespace Vulkan
{
	class LogicalDevice;


	class GraphicsPipeline : Utils::NoCopy
	{
	public:

		GraphicsPipeline(const LogicalDevice* logicalDevice, int w, int h);

	private:

		VkPipeline m_pipeline;

		Utils::ScopeGuard m_graphicsPipelineCleanup;

		ShaderStage m_vertexShader;
		ShaderStage m_fragmentShader;
		PipelineLayout m_pipelineLayout;

	};

}
