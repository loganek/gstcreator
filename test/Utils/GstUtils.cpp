/*
 * GstUtils.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: m.kolny
 */

#include "Utils/GstUtils.h"
#include <gtest/gtest.h>

using namespace Gst;
using Glib::RefPtr;

TEST(GstUtils, ShouldCorrectFindElementInBin)
{
	std::vector<std::string> path = { "first", "second" };

	auto model = Bin::create("main"),
			first = Bin::create(path[0]);
	first->add(ElementFactory::create_element("queue", path[1]));
	model->add(first);

	RefPtr<Object> o = GstUtils::find_element(path, model);

	ASSERT_EQ(path[1], o->get_name());
}

TEST(GstUtils, ShouldNotFindElementInBin)
{
	std::vector<std::string> path = { "first", "second" };

	auto model = Bin::create("main");
	model->add(ElementFactory::create_element("queue", path[1]));

	RefPtr<Object> o = GstUtils::find_element(path, model);

	ASSERT_FALSE(o);
}

TEST(GstUtils, ShouldReturnsTheSameObject)
{
	std::vector<std::string> path = {};

	auto model = Bin::create("main");
	RefPtr<Object> o = GstUtils::find_element(path, model);

	ASSERT_EQ(model, RefPtr<Bin>::cast_static(o));
}
