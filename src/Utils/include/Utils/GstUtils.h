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
#include <map>

class ObjectNodeInfo
{
public:
	typedef std::map<std::string, ObjectNodeInfo> MapInfo;
private:
	std::string value;
	MapInfo info_map;

public:
	ObjectNodeInfo() {}
	ObjectNodeInfo (const std::string& value) : value(value) {}
	std::string get_value() const { return value; }
	void insert(const std::string& key, const ObjectNodeInfo& obj)
	{ info_map.insert({key, obj}); }
	MapInfo get_map() const { return info_map; }

};
class GstUtils
{
private:
	static bool is_specified_dir_element(
			const Glib::RefPtr<Gst::Element>& element, Gst::PadDirection dir);
	static ObjectNodeInfo::MapInfo parse_pad_template(const Glib::RefPtr<Gst::PadTemplate>& tpl);
	static ObjectNodeInfo parse_caps(const Glib::RefPtr<Gst::Caps>& caps);

public:
	static Glib::RefPtr<Gst::Object> find_element(
			std::vector<std::string> path, const Glib::RefPtr<Gst::Element>& model);
	static std::vector<std::string> get_path(const Glib::RefPtr<Gst::Object>& obj,
			const Glib::RefPtr<Gst::Element>& model);
	static bool is_sink_element(const Glib::RefPtr<Gst::Element>& element);
	static bool is_src_element(const Glib::RefPtr<Gst::Element>& element);
	static std::map<std::string, ObjectNodeInfo> get_object_info(const Glib::RefPtr<Gst::Object>& object);
};

#endif /* GSTUTILS_H_ */
