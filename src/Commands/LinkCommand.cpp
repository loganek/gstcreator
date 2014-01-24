/*
 * gstcreator
 *     LinkCommand.cpp
 *
 *  Created on: 24 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "LinkCommand.h"

using namespace Gst;
using Glib::RefPtr;

LinkCommand::LinkCommand(const RefPtr<Element>& dst_element, const RefPtr<Element>& model)
: Command(model),
  dst_element(dst_element)
{
}

void LinkCommand::run_command()
{
	model->link(dst_element);
}

CommandType LinkCommand::get_type()
{
	return CommandType::LINK;
}




