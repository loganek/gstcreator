/*
 * gstcreator
 *     UnlinkCommand.cpp
 *
 *  Created on: 12 lut 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "UnlinkCommand.h"

using namespace Gst;
using Glib::RefPtr;

UnlinkCommand::UnlinkCommand(const RefPtr<Pad>& model)
: Command(model)
{
}

void UnlinkCommand::run_command()
{
	RefPtr<Pad> tmp = tmp.cast_static(model);
	RefPtr<Pad> src_pad = tmp->get_direction() == PAD_SRC ? src_pad : tmp->get_peer();

	if (!src_pad)
		throw std::runtime_error("Cannot unlink pad");

	RefPtr<Pad> sink_pad = src_pad->get_peer();

	if (!sink_pad)
		throw std::runtime_error("Cannot unlink pad: pad is not linked");

	if (!src_pad->unlink(sink_pad))
		throw std::runtime_error("Cannot unlink pad: unknow error");
}

CommandType UnlinkCommand::get_type()
{
	return CommandType::UNLINK;
}


