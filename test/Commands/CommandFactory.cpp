/*
 * gstcreator
 *     CommandFactory.cpp
 *
 *  Created on: 21 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Commands/CommandFactory.h"
#include "LexPars/Parser.h"
#include <gtest/gtest.h>

using namespace Gst;
using Glib::RefPtr;

TEST(CommandFactory, ShouldCorrectAddElement)
{
	auto bin = Bin::create("empty");
	auto parsed = Parser().parse("addelement(\"xvimagesink\", \"mojsink\")");


	CommandFactory factory(parsed, bin);
	std::shared_ptr<Command> cmd = factory.process();
	cmd->run_command();

	auto element = bin->get_element("mojsink");

	ASSERT_TRUE(element);
	ASSERT_EQ("mojsink", element->get_name());
}
