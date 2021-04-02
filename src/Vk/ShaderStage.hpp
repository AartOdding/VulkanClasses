#pragma once

#include <filesystem>

#include <vulkan/vulkan.h>

#include <Utils/NoCopy.hpp>
#include <Utils/ScopeGuard.hpp>



namespace Vulkan
{
	class LogicalDevice;
	
	struct ShaderStageSettings
	{
		VkShaderStageFlagBits stage{};
		std::filesystem::path path{};
	};


	class ShaderStage : Utils::NoCopy
	{
	public:

		ShaderStage(const LogicalDevice* logicalDevice, ShaderStageSettings settings);

		VkPipelineShaderStageCreateInfo pipelineStageCreateInfo() const;

	private:

		Utils::ScopeGuard m_shaderModuleCleanup;

		VkShaderModule m_shaderModule;
		VkShaderStageFlagBits m_stage;
	};

}
