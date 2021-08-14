#pragma once

#include <filesystem>

#include <vulkan/vulkan.h>

#include <Vulkan/Utils/NoCopy.hpp>
#include <Vulkan/Utils/ScopeGuard.hpp>



namespace Vulkan
{
	class Device;
	
	struct ShaderStageSettings
	{
		VkShaderStageFlagBits stage{};
		std::filesystem::path path{};
	};


	class ShaderStage : Utils::NoCopy
	{
	public:

		ShaderStage(const Device* logicalDevice, ShaderStageSettings settings);

		VkPipelineShaderStageCreateInfo pipelineStageCreateInfo() const;

	private:

		Utils::ScopeGuard m_shaderModuleCleanup;

		VkShaderModule m_shaderModule;
		VkShaderStageFlagBits m_stage;
	};

}
