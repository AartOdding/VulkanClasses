#pragma once

#include <vulkan/vulkan.h>

#include <Vulkan/Utils/NoCopy.hpp>
#include <Vulkan/Utils/ScopeGuard.hpp>



namespace Vulkan
{
	class LogicalDevice;


	class PipelineLayout : Utils::NoCopy
	{
	public:

		PipelineLayout(const LogicalDevice* logicalDevice);

		VkPipelineLayout get() const;

	private:

		VkPipelineLayout m_pipelineLayout;

		Utils::ScopeGuard m_pipelineLayoutCleanup;

	};

}
