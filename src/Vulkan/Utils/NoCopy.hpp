#pragma once


namespace Vulkan::Utils
{

	struct NoCopy
	{
        NoCopy() = default;

        NoCopy(const NoCopy&) = delete;
        NoCopy(NoCopy&&) = delete;

        NoCopy& operator=(const NoCopy&) = delete;
        NoCopy& operator=(NoCopy&&) = delete;
	};

}
