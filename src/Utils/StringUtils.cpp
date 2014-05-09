/*
 * gstcreator
 *     StringUtils.cpp
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "StringUtils.h"
#include <cstring>
#include <algorithm>

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

string StringUtils::join(const vector<string>& vect, const string& delim)
{
	string out;

	if (vect.empty())
		return out;

	for (size_t i = 0; i < vect.size() - 1; i++)
		out += vect[i] + delim;

	out += vect.back();

	return out;
}

string StringUtils::to_upper(string text)
{
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);
	return text;
}

string StringUtils::to_lower(string text)
{
	transform(text.begin(), text.end(), text.begin(), ::tolower);
	return text;
}

#define STR_TO_NUM(type, cpp_method) template<> \
type StringUtils::str_to_numeric(const std::string& value) \
{ \
	return cpp_method(value); \
}

STR_TO_NUM(int, stoi)
STR_TO_NUM(long, stol)
STR_TO_NUM(long long, stoll)
STR_TO_NUM(unsigned int, stoul)
STR_TO_NUM(unsigned long, stoul)
STR_TO_NUM(unsigned long long, stoull)
STR_TO_NUM(float, stof)
STR_TO_NUM(double, stod)
