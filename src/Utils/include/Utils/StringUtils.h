/*
 * gstcreator
 *     StringUtils.h
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <string>
#include <vector>

class StringUtils
{
public:
	static std::vector<std::string> split(const std::string& text,
			const std::string& delim);
	static std::string join(const std::vector<std::string>& vect,
			const std::string& delim);

	static std::string to_upper(std::string text);
	static std::string to_lower(std::string text);
};

#endif /* STRINGUTILS_H_ */
