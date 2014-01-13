/*
 * gstcreator
 *     AddCommand.h
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef ADDCOMMAND_H_
#define ADDCOMMAND_H_

#include "Command.h"

class AddCommand : public Command
{
private:
	Glib::RefPtr<Gst::Object> item;

public:
	AddCommand(const Glib::RefPtr<Gst::Object>& item,
			const Glib::RefPtr<Gst::Element>& model);

	void run_command();
	CommandType get_type();
};

#endif /* ADDCOMMAND_H_ */
