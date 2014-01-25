/*
 * gstcreator
 *     Command.h
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <gstreamermm.h>

enum class CommandType
{
	ADD,
	REMOVE,
	LINK,
	STATE
};


// todo move it to a lexer

enum class Token
{
	INVALID = 0,
	IDENTIFIER = 1,
	OPERATOR = 2,
	VAR_DEF = 4,
	EOL = 8,
};

// todo move it to a StateCommand

enum class StateType
{
	VOID_PENDING = 0,
	NULL_STATE,
	READY,
	PAUSED,
	PLAYING
};


class Command
{
protected:
	Glib::RefPtr<Gst::Object> model;

public:
	Command(const Glib::RefPtr<Gst::Object>& model);
	virtual ~Command() {}
	virtual void run_command() = 0;
	virtual CommandType get_type() = 0;
};

#endif /* COMMAND_H_ */
