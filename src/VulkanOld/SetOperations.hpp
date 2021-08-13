#pragma once

#include <unordered_set>


namespace Utils
{
	
	template<typename T>
	std::unordered_set<T> setUnion(const std::unordered_set<T>& lhs, const std::unordered_set<T>& rhs)
	{
		std::unordered_set<T> result;
		result.reserve(lhs.size() + rhs.size());
		result.insert(lhs.begin(), lhs.end());
		result.insert(rhs.begin(), rhs.end());
		return result;
	}

	template<typename T>
	std::unordered_set<T> setDifference(const std::unordered_set<T>& lhs, const std::unordered_set<T>& rhs)
	{
		std::unordered_set<T> result = lhs;

		for (const auto& item : lhs)
		{
			if (rhs.count(item))
			{
				result.erase(item);
			}
		}
		return result;
	}

	template<typename T>
	std::unordered_set<T> setIntersection(const std::unordered_set<T>& lhs, const std::unordered_set<T>& rhs)
	{
		std::unordered_set<T> result;

		for (const auto& item : lhs)
		{
			if (rhs.count(item))
			{
				result.insert(item);
			}
		}
		return result;
	}
}
