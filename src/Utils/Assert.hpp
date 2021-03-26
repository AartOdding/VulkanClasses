#pragma once

#include <stdexcept>
#include <string_view>


namespace Utils
{

	inline void Assert(bool value, const char * error)
	{
		if (!value)
		{
			throw std::runtime_error(error);
		}
	}

	inline void Assert(bool value, const std::string& error)
	{
		if (!value)
		{
			throw std::runtime_error(error);
		}
	}
}

