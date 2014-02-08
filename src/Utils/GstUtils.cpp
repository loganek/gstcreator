/*
 * GstUtils.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: m.kolny
 */

#include "GstUtils.h"
#include <iostream>
using namespace Gst;
using Glib::RefPtr;
using namespace std;

RefPtr<Object> GstUtils::find_element(vector<string> path,
		const RefPtr<Element>& model)
{
	if (path.size() == 0)
		return model;

	if (path.size() == 1)
	{
		if (model->is_bin())
		{
			RefPtr<Object> o = RefPtr<Bin>::cast_static(model)->get_child(path[0]);

			if (o) return o;
		}

		return model->get_static_pad(path[0]);
	}

	if (!model->is_bin()) return RefPtr<Object>();

	auto model_bin = RefPtr<Bin>::cast_static(model);
	auto child = model_bin->get_child(path[0]);

	if (!child)
		return child;

	path.erase(path.begin());
	return find_element(path, RefPtr<Element>::cast_static(child));
}

vector<string> GstUtils::get_path(const RefPtr<Object>& obj,
		const RefPtr<Element>& model)
{
	RefPtr<Element> current_element;
	vector<string> path;

	if (obj->is_pad())
	{
		current_element = RefPtr<Pad>::cast_static(obj)->get_parent_element();
		path.push_back(obj->get_name().c_str());
	}
	else if (!obj->is_element())
		throw std::runtime_error("cannot get path of non-element and non-pad object");
	else
		current_element = RefPtr<Element>::cast_static(obj);

	do
	{
		path.push_back(current_element->get_name().c_str());
		current_element = RefPtr<Element>::cast_static(current_element->get_parent());
	} while (current_element);

	reverse(path.begin(), path.end());

	return path;
}

bool GstUtils::is_specified_dir_element(const RefPtr<Element>& element, PadDirection dir)
{
	auto tpls = element->get_factory()->get_static_pad_templates();

	for (auto tpl : tpls)
	{
		if (tpl.get_direction() != dir)
			return false;
	}

	return true;
}
bool GstUtils::is_sink_element(const RefPtr<Element>& element)
{
	return is_specified_dir_element(element, PAD_SINK);
}
bool GstUtils::is_src_element(const RefPtr<Element>& element)
{
	return is_specified_dir_element(element, PAD_SRC);
}
