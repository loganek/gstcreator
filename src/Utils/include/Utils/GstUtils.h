/*
 * GstUtils.h
 *
 *  Created on: Jan 14, 2014
 *      Author: m.kolny
 */

#ifndef GSTUTILS_H_
#define GSTUTILS_H_

#include <gstreamermm.h>
#include <vector>
#include <string>

class GstUtils
{
public:
	static Glib::RefPtr<Gst::Object> find_element(
			std::vector<std::string> path, const Glib::RefPtr<Gst::Element>& model);
};

#endif /* GSTUTILS_H_ */
