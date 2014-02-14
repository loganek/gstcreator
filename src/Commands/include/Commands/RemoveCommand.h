/*
 * gstcreator
 *     RemoveCommand.h
 *
 *  Created on: 25 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef REMOVECOMMAND_H_
#define REMOVECOMMAND_H_

#include "Command.h"

class RemoveCommand: public Command
{
private:
	Glib::RefPtr<Gst::Object> item;

public:
	RemoveCommand(const Glib::RefPtr<Gst::Object>& item,
			const Glib::RefPtr<Gst::Element>& model);
	RemoveCommand(const Glib::RefPtr<Gst::Element>& model);

	void run_command();
	CommandType get_type();
};

#endif /* REMOVECOMMAND_H_ */
