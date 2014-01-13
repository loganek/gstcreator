/*
 * gstcreator
 *     AddCommand.cpp
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Commands/AddCommand.h"
#include <gtest/gtest.h>
#include <gstreamermm.h>
#include <memory>

using namespace Gst;
using Glib::RefPtr;

TEST(AddCommand, CheckCorrectTypeOfCommand)
{
	std::shared_ptr<Command> cmd(new AddCommand(RefPtr<Object>(), RefPtr<Element>()));
	ASSERT_EQ(CommandType::ADD, cmd->get_type());
}

TEST(AddCommand, ShouldThrowExceptionsBecauseOfWrongArguments)
{
	auto model = ElementFactory::create_element("tee"),
			item = ElementFactory::create_element("queue");
	std::shared_ptr<Command>cmd(new AddCommand(item, model));
	EXPECT_ANY_THROW(cmd->run_command());
}

TEST(AddCommand, ShouldCorrectAddPadToElement)
{
	Glib::ustring pad_name = "custom_pad";
	auto model = ElementFactory::create_element("tee");
	auto item = Pad::create(model->get_pad_template("src_%u"), pad_name);
	AddCommand(item, model).run_command();
	ASSERT_TRUE(model->get_static_pad(pad_name));
}

TEST(AddCommand, ShouldCorrectAddElementToBin)
{
	Glib::ustring element_name = "test_queue";
	auto item = ElementFactory::create_element("queue", element_name);
	auto model = Bin::create("test_bin");
	AddCommand(item, model).run_command();
	ASSERT_TRUE(model->get_element(element_name));
}
