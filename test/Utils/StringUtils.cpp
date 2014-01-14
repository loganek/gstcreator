/*
 * StringUtils.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: m.kolny
 */

#include "Utils/StringUtils.h"
#include <gtest/gtest.h>

using namespace std;

void check_split(const vector<string>& expected, const vector<string>& values)
{
	ASSERT_EQ(expected.size(), values.size());

	for (auto e_it = expected.begin(),
			v_it = values.begin(); e_it != expected.end();
			++e_it, v_it++)
		ASSERT_EQ(*e_it, *v_it);
}

TEST(StringUtils, ShouldCorrectSplitStringWithoutTraps)
{
	string text = "this _-_ is _-_ test";
	vector<string> expected = {"this", "is", "test"};
	vector<string> values = StringUtils::split(text, " _-_ ");

	check_split(expected, values);
}

TEST(StringUtils, ShouldCorrectSplitStringWithEmptyValues)
{
	string text = "this _-_ is _-_  _-_ test";
	vector<string> expected = {"this", "is", "test"};
	vector<string> values = StringUtils::split(text, " _-_ ");

	check_split(expected, values);
}

TEST(StringUtils, ShouldCorrectSplitStringWithEndingDelim)
{
	string text = "this _-_ is _-_ test _-_ ";
	vector<string> expected = {"this", "is", "test"};
	vector<string> values = StringUtils::split(text, " _-_ ");

	check_split(expected, values);
}
