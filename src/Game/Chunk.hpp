#pragma once

#include "BlockID.hpp"

#include <array>
#include <vector>


namespace Game
{
	class Chunk
	{
	public:


	private:

		using Storage = std::array<std::array<std::array<BlockID, 32>, 32>, 32>;

		Storage m_storage;

	};
}
