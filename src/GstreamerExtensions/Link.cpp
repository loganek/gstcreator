/*
 * Link.cpp
 *
 *  Created on: May 10, 2014
 *      Author: loganek
 */

#include "Link.h"

using namespace Gst;
using Glib::RefPtr;

Link::Link(const RefPtr<Object>& source, const RefPtr<Object>& destination)
: Bus(gst_bus_new()),
  source(source),
  destination(destination)
{
	set_name(source->get_name() + "->" + destination->get_name());
}

RefPtr<Object> Link::get_source()
{
	return source;
}

RefPtr<Object> Link::get_destination()
{
	return destination;
}
