/*
 * gstcreator
 *     StateCommand.cpp
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Commands/StateCommand.h"
#include <gtest/gtest.h>
#include <gstreamermm.h>
#include <memory>

using namespace Gst;
using Glib::RefPtr;

TEST(StateCommand, CheckCorrectTypeOfCommand)
{
	std::shared_ptr<Command> cmd(new StateCommand(StateType::PLAYING, RefPtr<Element>()));
	ASSERT_EQ(CommandType::STATE, cmd->get_type());
}

TEST(StateCommand, ShouldCorrectChangeState)
{
	auto element = ElementFactory::create_element("queue");
	StateCommand(StateType::PLAYING, element).run_command();
	State state, pending;
	element->get_state(state, pending, 0);
	ASSERT_EQ(STATE_PLAYING, state);
	StateCommand(StateType::NULL_STATE, element).run_command();
	element->get_state(state, pending, 0);
	ASSERT_EQ(STATE_NULL, state);
}
