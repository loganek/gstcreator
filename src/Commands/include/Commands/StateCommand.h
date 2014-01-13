/*
 * gstcreator
 *     StateCommand.h
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef STATECOMMAND_H_
#define STATECOMMAND_H_

#include "Command.h"

enum class StateType : int
{
	VOID_PENDING = 0,
	NULL_STATE,
	READY,
	PAUSED,
	PLAYING
};

class StateCommand : public Command
{
private:
	StateType state;

public:
	StateCommand(StateType state, const Glib::RefPtr<Gst::Element>& model);
	void run_command();
	CommandType get_type();
};

#endif /* STATECOMMAND_H_ */
