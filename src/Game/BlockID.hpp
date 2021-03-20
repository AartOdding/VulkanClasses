#pragma once

#include <cstdint>


namespace Game
{
	enum class BlockID : std::uint8_t
	{
		Air = 0,
		Dirt = 1,



		Other = 255
	};
}
