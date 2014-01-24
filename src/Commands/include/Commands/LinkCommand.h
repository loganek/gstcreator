/*
 * gstcreator
 *     LinkCommand.h
 *
 *  Created on: 24 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef LINKCOMMAND_H_
#define LINKCOMMAND_H_

#include "Command.h"

class LinkCommand : public Command
{
private:
	Glib::RefPtr<Gst::Object> dst_element;

	void link_pads(const Glib::RefPtr<Gst::Pad>& src_pad, const Glib::RefPtr<Gst::Pad>& dst_pad);

public:
	LinkCommand(const Glib::RefPtr<Gst::Object>& dst_element, const Glib::RefPtr<Gst::Object>& model);
	void run_command();
	CommandType get_type();

};

#endif /* LINKCOMMAND_H_ */
