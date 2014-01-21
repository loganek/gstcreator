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
	auto parsed = Parser().parse("add_element(\"xvimagesink\", \"mojsink\")");


	CommandFactory factory(parsed, bin);
	std::shared_ptr<Command> cmd = factory.process();
	cmd->run_command();

	auto element = bin->get_element("mojsink");

	ASSERT_TRUE(element);
	ASSERT_EQ("mojsink", element->get_name());
}

TEST(CommandFactory, ShouldCorrectAddPad)
{
	auto tee = ElementFactory::create_element("tee", "test_tee");
	auto parsed = Parser().parse("add_pad(\"src_%u\", \"moj_pad\")");


	CommandFactory factory(parsed, tee);
	std::shared_ptr<Command> cmd = factory.process();
	cmd->run_command();

	auto pad = tee->get_static_pad("moj_pad");

	ASSERT_TRUE(pad);
	ASSERT_EQ("moj_pad", pad->get_name());
}
