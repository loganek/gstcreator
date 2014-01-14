/*
 * gstcreator
 *     StringUtils.cpp
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "StringUtils.h"
#include <cstring>

using namespace std;

vector<string> StringUtils::split(const string& text, const string& delim)
{
	// TODO support arguments with " "

	char* cstr = const_cast<char*>(text.c_str()),
			*current;
	vector<string> values;

	current = strtok(cstr, delim.c_str());
	while (current)
	{
		values.push_back(current);
		current = strtok(nullptr, delim.c_str());
	}
	return values;
}
