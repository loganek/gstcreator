/*
 * gstcreator
 *     RemoveCommand.cpp
 *
 *  Created on: 25 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "RemoveCommand.h"

using namespace Gst;
using Glib::RefPtr;

RemoveCommand::RemoveCommand(const RefPtr<Object>& item, const RefPtr<Element>& model)
: Command(model),
  item(item)
{
}

RemoveCommand::RemoveCommand(const RefPtr<Element>& model)
: Command(model)
{
}

void RemoveCommand::run_command()
{
	if (!item)
	{
		if (model->get_parent())
			RefPtr<Bin>::cast_static(model->get_parent())->remove(RefPtr<Element>::cast_static(model));
		else
			throw std::runtime_error("cannot remove element without parent (including main pipeline)");
	}
	else if (item->is_element() && model->is_bin())
		RefPtr<Bin>::cast_static(model)->remove(RefPtr<Element>::cast_static(item));
	else if (item->is_pad() && model->is_element())
	{
		if (!RefPtr<Element>::cast_static(model)->remove_pad(RefPtr<Pad>::cast_static(item)))
			throw std::runtime_error("cannot remove pad");
	}
	else
		throw std::runtime_error("invalid item type");
}

CommandType RemoveCommand::get_type()
{
	return CommandType::REMOVE;
}

