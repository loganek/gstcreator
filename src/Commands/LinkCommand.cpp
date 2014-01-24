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

LinkCommand::LinkCommand(const RefPtr<Object>& dst_element, const RefPtr<Object>& model)
: Command(model),
  dst_element(dst_element)
{
}

void LinkCommand::run_command()
{
	if (model->is_element() && dst_element->is_element())
		RefPtr<Element>::cast_static(model)->link(RefPtr<Element>::cast_static(dst_element));
	else if (model->is_pad() && dst_element->is_pad())
		link_pads(RefPtr<Pad>::cast_static(model), RefPtr<Pad>::cast_static(dst_element));
	else if (model->is_element() && dst_element->is_pad())
	{
		RefPtr<Pad> dst_pad = dst_pad.cast_static(dst_element);
		RefPtr<Pad> src_pad = RefPtr<Element>::cast_static(model)->create_compatible_pad(dst_pad, Caps::create());

		if (!src_pad)
			throw std::runtime_error("Cannot find compatible pads in element");

		link_pads(src_pad, dst_pad);
	}
	else if (model->is_pad() && dst_element->is_element())
	{
		RefPtr<Pad> src_pad = src_pad.cast_static(model);
		RefPtr<Pad> dst_pad = RefPtr<Element>::cast_static(dst_element)->create_compatible_pad(src_pad, Caps::create());

		if (!dst_pad)
			throw std::runtime_error("Cannot find compatible pads in element");

		link_pads(src_pad, dst_pad);
	}
}

void LinkCommand::link_pads(const RefPtr<Pad>& src_pad, const RefPtr<Pad>& dst_pad)
{
	PadLinkReturn ret = src_pad->link(dst_pad);

	if (ret != PAD_LINK_OK)
		throw std::runtime_error("Cannot link pads: " + std::to_string(ret));
}

CommandType LinkCommand::get_type()
{
	return CommandType::LINK;
}
