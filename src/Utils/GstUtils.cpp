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


ObjectNodeInfo GstUtils::parse_caps(const RefPtr<Caps>& caps)
{
	Glib::ustring caps_str;

	bool finish = true;

	if (!caps || caps->empty())
		caps_str = "EMPTY";
	else if (caps->is_any())
		caps_str = "ANY";
	else finish = false;

	if (finish)
		return ObjectNodeInfo(caps_str);

	ObjectNodeInfo ret;

	for (guint i = 0; i < caps->size(); i++)
	{
		Structure structure = caps->get_structure(i);
		ObjectNodeInfo obj;

		for (int j = 0; j < structure.size(); j++)
		{
			Glib::ustring field_name = structure.get_nth_field_name(j);
			Glib::ValueBase base;

			structure.get_field(field_name, base);
			obj.insert(field_name, ObjectNodeInfo(gst_value_serialize(base.gobj())));
		}
		ret.insert(structure.get_name(), obj);
	}


	return ret;
}

ObjectNodeInfo::MapInfo GstUtils::parse_pad_template(const RefPtr<PadTemplate>& tpl)
{
	auto dir = tpl->get_direction();
	auto presence = tpl->get_presence();
	return {
			{"template name", std::string(tpl->get_name_template().c_str())},
			{"caps", parse_caps(tpl->get_caps())},
			{"direction", std::string(dir == PAD_SINK ? "SINK" : (dir == PAD_SRC ? "SRC" : "UNKNOWN"))},
			{"direction", std::string(presence == PAD_ALWAYS ? "ALWAYS" : (presence == PAD_REQUEST ? "REQUEST" : "SOMETIMES"))}
	};
}

std::map<std::string, ObjectNodeInfo> GstUtils::get_object_info(const Glib::RefPtr<Gst::Object>& object)
{
	ObjectNodeInfo::MapInfo info_map;

	if (object->is_pad())
	{
		info_map.insert({"type", std::string("PAD")});
		RefPtr<Pad> obj_pad = obj_pad.cast_static(object);
		info_map.insert({"current caps", parse_caps(obj_pad->get_current_caps())});
		info_map.insert({"allowed caps", parse_caps(obj_pad->get_allowed_caps())});
		auto dir = obj_pad->get_direction();
		info_map.insert({"direction", std::string(dir == PAD_SINK ? "SINK" : (dir == PAD_SRC ? "SRC" : "UNKNOWN"))});
		info_map.insert({"peer", std::string(obj_pad->get_peer() ? obj_pad->get_peer()->get_name() : "null")});

		ObjectNodeInfo tpl_node("pad template");
		for (auto i : parse_pad_template(obj_pad->get_pad_template()))
			tpl_node.insert(i.first, i.second);
		info_map.insert({"pad template", tpl_node});
	}
	else if (object->is_element())
	{
		if (object->is_bin())
		{
			info_map.insert({"type", std::string("BIN")});
			RefPtr<Bin> obj_bin = obj_bin.cast_static(object);
			info_map.insert({"children count", std::to_string(obj_bin->get_num_children())});
		}
		else
			info_map.insert({"type", std::string("ELEMENT")});
		RefPtr<Element> obj_element = obj_element.cast_static(object);
		info_map.insert({"factory", std::string(obj_element->get_factory()->get_name().c_str())});
	}
	else if (object->is_pad_template())
	{
		info_map.insert({"type", std::string("PAD TEMPLATE")});
		RefPtr<PadTemplate> obj_pad_template = obj_pad_template.cast_static(object);
		for (auto i : parse_pad_template(obj_pad_template))
			info_map.insert(i);
	}

	info_map.insert({"name", ObjectNodeInfo(object->get_name())});
	info_map.insert({"path", ObjectNodeInfo(object->get_path_string())});
	info_map.insert({"parent", ObjectNodeInfo(object->get_parent() ? object->get_parent()->get_name() : "null")});


	return info_map;
}
