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
};

class Command
{
protected:
	Glib::RefPtr<Gst::Element> model;

public:
	Command(const Glib::RefPtr<Gst::Element>& model);
	virtual ~Command() {}
	virtual void run_command() = 0;
	virtual CommandType get_type() = 0;
};

#endif /* COMMAND_H_ */
