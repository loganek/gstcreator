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

RefPtr<Element> GstUtils::find_element(vector<string> path,
		const RefPtr<Bin>& model)
{
	RefPtr<Element> child = child.cast_static(model->get_child(path[0]));

	if (!child)
		return child;

	if (path.size() == 1)
		return child;
	else if (!child->is_bin())
		return RefPtr<Element>();

	path.erase(path.begin());
	return find_element(path, RefPtr<Bin>::cast_static(child));
}
