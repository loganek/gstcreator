/*
 * gstcreator
 *     GstController.h
 *
 *  Created on: 21 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef GSTCONTROLLER_H_
#define GSTCONTROLLER_H_

#include <gstreamermm.h>
#include <set>

class IModelObserver
{
public:
	virtual ~IModelObserver(){}
	virtual void pad_added(const Glib::RefPtr<Gst::Pad>& pad) = 0;
	virtual void pad_removed(const Glib::RefPtr<Gst::Pad>& pad) = 0;
	virtual void pad_linked(const Glib::RefPtr<Gst::Pad>& proxy_pad) = 0;
	virtual void pad_unlinked(const Glib::RefPtr<Gst::Pad>& proxy_pad) = 0;
	virtual void element_added(const Glib::RefPtr<Gst::Element>& element) = 0;
	virtual void element_removed(const Glib::RefPtr<Gst::Element>& element) = 0;

};

class GstController
{
private:
	const Glib::RefPtr<Gst::Pipeline> master_model;
	Glib::RefPtr<Gst::Bin> current_model;

	std::set<IModelObserver*> observers;

	void set_watch_method(const Glib::RefPtr<Gst::Element>& element);

	template<typename ...Args>
	void notify_observers(void (IModelObserver::* fun)(Args...), Args... args)
	{
		for (auto observer: observers)
			(observer->*fun)(args...);
	}

public:
	GstController(const Glib::RefPtr<Gst::Pipeline>& master_model);
	virtual ~GstController(){}

	Glib::RefPtr<Gst::Pipeline> get_master_model() const;
	Glib::RefPtr<Gst::Bin> get_current_model() const;

	void update_current_model(const Glib::RefPtr<Gst::Bin>& model);
	void export_bin_to_file(const std::string& filename, int graph_details, bool current_model);

	void register_model_observer(IModelObserver* observer);
	void unregister_model_observer(IModelObserver* observer);
};

#endif /* GSTCONTROLLER_H_ */
