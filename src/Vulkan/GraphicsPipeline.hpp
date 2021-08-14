#pragma once

#include <stdint.h>

#include <vulkan/vulkan.h>

#include <Vulkan/Utils/NoCopy.hpp>
#include <Vulkan/Utils/ScopeGuard.hpp>
#include <Vulkan/PipelineLayout.hpp>
#include <Vulkan/ShaderStage.hpp>



namespace Vulkan
{
	class Device;


	class GraphicsPipeline : Utils::NoCopy
	{
	public:

		GraphicsPipeline(const Device* logicalDevice, uint32_t w, uint32_t h);

	private:

		VkPipeline m_pipeline;

		Utils::ScopeGuard m_graphicsPipelineCleanup;

		ShaderStage m_vertexShader;
		ShaderStage m_fragmentShader;
		PipelineLayout m_pipelineLayout;

	};

}
