/*
 * gstcreator
 *     StringUtils.cpp
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "StringUtils.h"
#include <algorithm>

std::vector<std::string> StringUtils::split(std::string text,
		const std::string& delim)
{
	size_t pos;
	std::vector<std::string> values;

	while ((pos = text.find(delim)) != std::string::npos)
	{
		// TODO support arguments with " "
		std::string v = text.substr(0, pos);
		if (!v.empty())
			values.push_back(v);
		text.erase(0, pos + 1);
	}

	values.push_back(text);

	return values;
}
