#pragma once

#include <algorithm>
#include <iterator>
#include <set>
#include <unordered_set>
#include <vector>


namespace Utils
{

	template<typename StdStringContainer>
	std::vector<const char*> getCStringList(const StdStringContainer& strings)
	{
		std::vector<const char*> cStrings;
		cStrings.reserve(strings.size());

		for (const auto& string : strings)
		{
			cStrings.push_back(string.c_str());
		}

		return cStrings;
	}

	template<typename T>
	std::set<T> setUnion(const std::set<T>& lhs, const std::set<T>& rhs)
	{
		std::set<T> result;
		std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(result, result.begin()));
		return result;
	}

	template<typename T>
	std::set<T> setDifference(const std::set<T>& lhs, const std::set<T>& rhs)
	{
		std::set<T> result;
		std::set_difference(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(result, result.begin()));
		return result;
	}

	template<typename T>
	std::set<T> setIntersection(const std::set<T>& lhs, const std::set<T>& rhs)
	{
		std::set<T> result;
		std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(result, result.begin()));
		return result;
	}

	
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
