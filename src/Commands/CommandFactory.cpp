/*
 * gstcreator
 *     CommandFactory.cpp
 *
 *  Created on: 21 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "CommandFactory.h"
#include "Commands.h"
#include "Utils/GstUtils.h"

using Glib::RefPtr;
using namespace Gst;
using namespace std;

CommandFactory::CommandFactory(ObjectExpression* object, const RefPtr<Element>& model)
: object(object),
  model(model),
  method(nullptr)
{
}

shared_ptr<Command> CommandFactory::process()
{
	vector<string> path;
	RefPtr<Element> current_model = model;
	ObjectExpression* temp_obj = object;

	while (temp_obj->get_child() != nullptr && !temp_obj->is_method())
	{
		path.push_back(temp_obj->get_name());
		temp_obj = temp_obj->get_child();
	}

	if (temp_obj->is_method())
	{
		gst_object = GstUtils::find_element(path, model);

		if (!gst_object)
			throw runtime_error("Cannot find element");

		method = temp_obj->as_method();
		return process_method();
	}

	throw runtime_error("cannot process object");
}

shared_ptr<Command> CommandFactory::process_method()
{
	if (method->get_name() == "add_element")
	{
		if (method->get_args().size() != 2 && method->get_args().size() != 1)
			throw runtime_error("invalid number of arguments");

		if (!gst_object->is_bin())
			throw runtime_error("invalid object type");

		RefPtr<Element> element;

		if (method->get_args().size() == 1)
		{
			// TODO it might be a gstreamer object or string
			element = ElementFactory::create_element(method->get_args()[0]->get_name());
		}
		else
		{
			if (method->get_args()[1]->get_type() != ExpressionType::STRING)
				throw runtime_error("second argument met be a string");

			element = ElementFactory::create_element(method->get_args()[0]->get_name(), method->get_args()[1]->get_name());
		}

		return shared_ptr<Command>(new AddCommand(element, RefPtr<Bin>::cast_static(gst_object)));

	}
	else if (method->get_name() == "add_pad")
	{
		int args_cnt = method->get_args().size();

		if (args_cnt != 1 && args_cnt != 2)
			throw runtime_error("invalid number of arguments");

		if (!gst_object->is_element())
			throw runtime_error("invalid object type");

		RefPtr<Element> el = el.cast_static(gst_object);
		RefPtr<PadTemplate> tpl = el->get_pad_template(method->get_args()[0]->get_name().c_str());
		RefPtr<Pad> pad = (args_cnt == 1) ? Pad::create(tpl) : Pad::create(tpl, method->get_args()[1]->get_name().c_str());

		return shared_ptr<Command>(new AddCommand(pad, el));
	}

	throw runtime_error("invalid method");
}
