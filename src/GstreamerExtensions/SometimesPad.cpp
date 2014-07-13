/*
 * SometimesPad.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: loganek
 */

#include "SometimesPad.h"

using namespace Gst;
using Glib::RefPtr;

RefPtr<Pad> SometimesPad::create(const RefPtr<const PadTemplate>& pad_template, const RefPtr<Object>& parent)
{
	auto pad = Pad::create(pad_template);
	pad->set_parent(parent);

	return pad;
}

