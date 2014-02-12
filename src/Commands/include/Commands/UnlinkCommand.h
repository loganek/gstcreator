/*
 * gstcreator
 *     UnlinkCommand.h
 *
 *  Created on: 12 lut 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef UNLINKCOMMAND_H_
#define UNLINKCOMMAND_H_

#include "Command.h"

class UnlinkCommand : public Command
{
public:
	UnlinkCommand(const Glib::RefPtr<Gst::Pad>& model);
	void run_command();
	CommandType get_type();

};

#endif /* UNLINKCOMMAND_H_ */
