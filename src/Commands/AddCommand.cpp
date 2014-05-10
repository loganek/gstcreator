/*
 * gstcreator
 *     AddCommand.cpp
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "AddCommand.h"

using namespace Gst;
using Glib::RefPtr;

AddCommand::AddCommand(const RefPtr<Object>& item, const RefPtr<Element>& model)
: Command(model),
  item(item)
{
}

void AddCommand::run_command()
{
	if (item->is_element())
	{
		if (!model->is_bin())
			throw std::runtime_error("parent object must be a BIN object!");

		auto new_element = RefPtr<Element>::cast_static(item);
		auto parent = RefPtr<Bin>::cast_static(model);
		parent->add(new_element);
		State state, pending;
		parent->get_state(state, pending, 0);
		new_element->set_state(state);
	}

	else if (item->is_pad() && model->is_element())
	{
		if (!RefPtr<Element>::cast_static(model)->add_pad(RefPtr<Pad>::cast_static(item)))
			throw std::runtime_error("cannot add pad");
	}
	else if (item->is_pad_template() && model->is_element())
	{
		if (!RefPtr<Element>::cast_static(model)->request_pad(RefPtr<PadTemplate>::cast_static(item)))
			throw std::runtime_error("cannot add request pad");
	}
	else
		throw std::runtime_error("unknow item type");
}

CommandType AddCommand::get_type()
{
	return CommandType::ADD;
}
