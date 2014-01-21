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
		path.push_back(object->get_name());
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
	if (method->get_name() == "addelement")
	{
		if (method->get_args().size() != 2)
			throw runtime_error("invalid number of arguments");

		if (!gst_object->is_bin())
			throw runtime_error("invalid object type");

		return shared_ptr<Command>(new AddCommand(ElementFactory::create_element(method->get_args()[0]->get_name(),
				method->get_args()[1]->get_name()), RefPtr<Bin>::cast_static(gst_object)));
	}

	throw runtime_error("invalid method");
}
