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
