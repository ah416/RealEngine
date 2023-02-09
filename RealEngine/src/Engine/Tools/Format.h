#pragma once

#include <string>
#include <vector>
#include <sstream>
// debugging
#include <stdio.h>

#include <stdexcept>

#ifdef USE_STL_FORMAT
#include <format>
#endif

namespace fmt
{
#ifndef USE_STL_FORMAT
	template <typename FormatString>
	void FindBracketPositions(const FormatString& str, std::vector<int>& positions, int prev_length)
	{
		std::string current_str = str;
		size_t position = current_str.find('{');
		if (position == std::string::npos)
			return;
		std::string next = current_str.substr(position + 1);
		int next_len = (int)current_str.size() - (int)next.size();
		next_len += prev_length;
		position += prev_length;
		positions.push_back((int)position);
		FindBracketPositions(next, positions, next_len);
	}

	template <typename... Args>
	constexpr void ToString(std::vector<std::string>& string_args, Args &&...args)
	{
		([&](auto input)
		{
				std::stringstream ss;
		ss << input;
		string_args.push_back(ss.str());
		} (args), ...);
	}

	template <typename FormatString, typename... Args>
	std::string EmbedArguments(const FormatString& str, Args &&...args)
	{
		std::vector<int> positions;
		FindBracketPositions(str, positions, 0);

		if (sizeof...(args) < positions.size())
			throw std::runtime_error("Not enough arguments for the formatting string given!");

		std::string current_str = std::string(str);
		std::vector<std::string> string_args;
		string_args.reserve(sizeof...(args));

		ToString(string_args, std::forward<Args>(args)...);

		int index = 0;
		int offset = 0;
		for (auto& pos : positions)
		{
			// erase the two brackets
			current_str.erase(pos + offset, 2);

			// insert the argument
			current_str.insert(pos + offset, string_args[index]);

			offset -= 2; // subtract 2 for the two brackets
			offset += (int)string_args[index].size(); // add the size of the stringified arg
			index++;
		}

		return current_str;
	}

	template <typename FormatString, typename... Args>
	std::string format(const FormatString& str, Args &&...args)
	{
		std::string std_str = str;
		auto fmt_str = EmbedArguments(std_str, std::forward<Args>(args)...);

		return fmt_str;
	}
#endif

#ifdef USE_STL_FORMAT
	template <typename FormatString, typename... Args>
	std::string format(const FormatString& str, Args &&...args)
	{
		return std::vformat(str, std::make_format_args(args...));
	}
#endif
}