/*
 * gstcreator
 *     GstController.cpp
 *
 *  Created on: 21 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "GstController.h"

using Glib::RefPtr;
using namespace Gst;

GstController::GstController(const Glib::RefPtr<Gst::Pipeline>& master_model)
: master_model(master_model),
  current_model(master_model)
{
	set_watch_method(master_model);

	master_model->get_bus()->add_watch(sigc::mem_fun(this, &GstController::bus_method));
}

bool GstController::bus_method(const RefPtr<Bus>& bus, const RefPtr<Message>& message)
{
	switch (message->get_message_type())
	{
	case MESSAGE_STATE_CHANGED:
	{
		auto state = RefPtr<MessageStateChanged>::cast_static(message)->parse();
		if (message->get_source()->is_element())
			notify_observers<const RefPtr<Element>&, Gst::State>(&IModelObserver::state_changed,
					RefPtr<Element>::cast_static(message->get_source()), state);
		break;
	}
	default:
		break;
	}

	return true;
}

void GstController::update_current_model(const RefPtr<Bin>& model)
{
	RefPtr<Object> parent = model->get_parent();

	while (parent && parent != master_model)
	{
		parent = parent->get_parent();
	}

	if (!parent && model != master_model)
		throw std::runtime_error("Model is not an descendant of a master model");

	current_model = model;
}

Glib::RefPtr<Gst::Pipeline> GstController::get_master_model() const
{
	return master_model;
}

Glib::RefPtr<Gst::Bin> GstController::get_current_model() const
{
	return current_model;
}

void GstController::export_bin_to_file(const std::string& filename, int graph_details, bool is_current_model)
{
	gst_debug_bin_to_dot_file(is_current_model ? current_model->gobj() : GST_BIN(master_model->gobj()),
			(GstDebugGraphDetails)graph_details, filename.c_str());
}

void GstController::register_model_observer(IModelObserver* observer)
{
	if (!observers.insert(observer).second)
		throw std::runtime_error("Cannot register observer");
}

void GstController::unregister_model_observer(IModelObserver* observer)
{
	if (!observers.erase(observer))
		throw std::runtime_error("Cannot unregister observer");
}

void GstController::set_watch_method(const RefPtr<Element>& element)
{
	element->signal_pad_added().connect([this](const RefPtr<Pad>& pad){
		notify_observers<const RefPtr<Pad>&>(&IModelObserver::pad_added, pad);

		pad->signal_linked().connect([this](const RefPtr<Pad>& proxy_pad){
			notify_observers<const RefPtr<Pad>&>(&IModelObserver::pad_linked, proxy_pad);
		});
		pad->signal_unlinked().connect([this](const RefPtr<Pad>& proxy_pad){
			notify_observers<const RefPtr<Pad>&>(&IModelObserver::pad_unlinked, proxy_pad);
		});
	});
	element->signal_pad_removed().connect([this](const RefPtr<Pad>& pad){
		notify_observers<const RefPtr<Pad>&>(&IModelObserver::pad_removed, pad);
	});

	auto iterator = element->iterate_pads();
	while (iterator.next())
	{
		iterator->signal_linked().connect([this](const RefPtr<Pad>& proxy_pad){
			notify_observers<const RefPtr<Pad>&>(&IModelObserver::pad_linked, proxy_pad);
		});
		iterator->signal_unlinked().connect([this](const RefPtr<Pad>& proxy_pad){
			notify_observers<const RefPtr<Pad>&>(&IModelObserver::pad_unlinked, proxy_pad);
		});
	}


	if (element->is_bin())
	{
		RefPtr<Bin> bin = bin.cast_static(element);
		bin->signal_element_added().connect([this](const RefPtr<Element>& element){
			notify_observers<const RefPtr<Element>&>(&IModelObserver::element_added, element);
			set_watch_method(element);
		});
		bin->signal_element_removed().connect([this, bin](const RefPtr<Element>& element){
			notify_observers<const RefPtr<Element>&, const RefPtr<Bin>&>(&IModelObserver::element_removed, element, bin);
		});
	}
}
