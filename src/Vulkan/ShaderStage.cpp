#include <iostream>

#include <Vulkan/Utils/FileUtils.hpp>
#include <Vulkan/LogicalDevice.hpp>
#include <Vulkan/ShaderStage.hpp>



namespace Vulkan
{

	ShaderStage::ShaderStage(const LogicalDevice* logicalDevice, ShaderStageSettings settings)
		: m_stage(settings.stage)
	{
		auto codeBytes = Utils::readFileBytes(settings.path);

		VkShaderModuleCreateInfo moduleCreateInfo{};
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.codeSize = codeBytes.size();
		moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(codeBytes.data());

		if (vkCreateShaderModule(logicalDevice->get(), &moduleCreateInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module.");
		}
		else
		{
			m_shaderModuleCleanup = [device = logicalDevice->get(), shaderModule = m_shaderModule]()
			{
				std::cout << "Destroying Shader Module." << std::endl;
				vkDestroyShaderModule(device, shaderModule, nullptr);
			};
		}
	}

	VkPipelineShaderStageCreateInfo ShaderStage::pipelineStageCreateInfo() const
	{
		VkPipelineShaderStageCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.stage = m_stage;
		info.module = m_shaderModule;
		info.pName = "main";
		return info;
	}

}

