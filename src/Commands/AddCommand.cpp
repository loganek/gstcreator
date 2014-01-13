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
		if (!item->is_bin())
			throw std::runtime_error("parent object must be a BIN object!");

		RefPtr<Bin>::cast_static(model)->add(RefPtr<Element>::cast_static(item));
	}

	else if (item->is_pad())
	{
		if (model->add_pad(RefPtr<Pad>::cast_static(item)))
			throw std::runtime_error("cannot add pad");
	}
}

CommandType AddCommand::get_type()
{
	return CommandType::ADD;
}
