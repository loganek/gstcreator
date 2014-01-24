/*
 * gstcreator
 *     CommandFactory.cpp
 *
 *  Created on: 21 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "CommandFactory.h"
#include "Utils/GstUtils.h"
#include "Utils/StringUtils.h"

using Glib::RefPtr;
using namespace Gst;
using namespace std;

CommandFactory::CommandFactory(ObjectExpression* object, const RefPtr<Element>& model)
: model(model),
  object(object),
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

void CommandFactory::assert_argument_count(const vector<int>& expected, int actual)
{
	if (find(expected.begin(), expected.end(), actual) == expected.end())
	{
		vector<string> str_vect;

		for (auto e : expected)
			str_vect.push_back(to_string(e));

		throw runtime_error("Invalid number of arguments. Is: " + to_string(actual) +
				" but expected " + StringUtils::join(str_vect, ", ") + ".");
	}
}

shared_ptr<Command> CommandFactory::process_method()
{
	if (method->get_name() == "add_element")
		return process_add_element();
	else if (method->get_name() == "add_pad")
		return process_add_pad();

	throw runtime_error("invalid method");
}

shared_ptr<AddCommand> CommandFactory::process_add_element()
{
	assert_argument_count({1, 2}, method->get_args().size());

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

	return shared_ptr<AddCommand>(new AddCommand(element, RefPtr<Bin>::cast_static(gst_object)));
}

shared_ptr<AddCommand> CommandFactory::process_add_pad()
{
	int args_cnt = method->get_args().size();

	assert_argument_count({1, 2}, method->get_args().size());

	if (!gst_object->is_element())
		throw runtime_error("invalid object type");

	RefPtr<Element> el = el.cast_static(gst_object);
	RefPtr<PadTemplate> tpl = el->get_pad_template(method->get_args()[0]->get_name().c_str());
	RefPtr<Pad> pad = (args_cnt == 1) ? Pad::create(tpl) : Pad::create(tpl, method->get_args()[1]->get_name().c_str());

	return shared_ptr<AddCommand>(new AddCommand(pad, el));
}

std::shared_ptr<StateCommand> CommandFactory::process_state_command()
{
	assert_argument_count({1}, method->get_args().size());

	if (!gst_object->is_element())
		throw runtime_error("invalid object type");

	if (method->get_args()[0]->get_type() != ExpressionType::STRING)
		throw runtime_error("argument met be a string");

	StateType state; // TODO = from_string(method->get_args()[0]);

	return shared_ptr<StateCommand>(new StateCommand(state, RefPtr<Element>::cast_static(gst_object)));
}
