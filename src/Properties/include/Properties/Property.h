/*
 * gstcreator
 *     Property.h
 *
 *  Created on: 16 lut 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_

#include "Logic/IModelObserver.h"
#include <QWidget>
#include <gstreamermm.h>

class Property : public IModelObserver
{
protected:
	GParamSpec* param_spec;
	Glib::RefPtr<Gst::Element> element;
	QWidget* widget;

	virtual void build_widget() = 0;
	virtual void update_widget() = 0;

public:
	Property(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element);
	virtual ~Property();

	void property_changed(const Glib::RefPtr<Gst::Element>& element, const std::string& name);

	QWidget* get_widget();

	static QWidget* build_property_widget(const Glib::RefPtr<Gst::Element>& element);
	static Property* build_property(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element, const std::string& str_value);
};


#endif /* PROPERTY_H_ */
