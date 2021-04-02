#pragma once

#include <filesystem>
#include <fstream>
#include <vector>


namespace Utils
{

	inline std::vector<char> readFileBytes(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file!");
		}

		int fileSize = file.tellg();

		std::vector<char> buffer;
		buffer.resize(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		return buffer;
	}

}
