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
private:
	static bool is_specified_dir_element(
			const Glib::RefPtr<Gst::Element>& element, Gst::PadDirection dir);
public:
	static Glib::RefPtr<Gst::Object> find_element(
			std::vector<std::string> path, const Glib::RefPtr<Gst::Element>& model);
	static std::vector<std::string> get_path(const Glib::RefPtr<Gst::Object>& obj,
			const Glib::RefPtr<Gst::Element>& model);
	static bool is_sink_element(const Glib::RefPtr<Gst::Element>& element);
	static bool is_src_element(const Glib::RefPtr<Gst::Element>& element);
};

#endif /* GSTUTILS_H_ */
