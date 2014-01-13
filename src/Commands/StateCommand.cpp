/*
 * gstcreator
 *     StateCommand.cpp
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "StateCommand.h"

using namespace Gst;
using Glib::RefPtr;

StateCommand::StateCommand(StateType state, const RefPtr<Element>& model)
: Command(model),
  state(state)
{
}

void StateCommand::run_command()
{
	StateChangeReturn ret = model->set_state(static_cast<State>(state));

	if (ret == STATE_CHANGE_FAILURE)
		throw std::runtime_error("cannot change state");
}

CommandType StateCommand::get_type()
{
	return CommandType::STATE;
}

