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

class GstController
{
private:
	const Glib::RefPtr<Gst::Pipeline> master_model;
	Glib::RefPtr<Gst::Element> current_model;

public:
	GstController(const Glib::RefPtr<Gst::Pipeline>& master_model);
	virtual ~GstController(){}

	Glib::RefPtr<Gst::Pipeline> get_master_model() const;
	Glib::RefPtr<Gst::Element> get_current_model() const;

	void update_current_model(const Glib::RefPtr<Gst::Element>& model);
};

#endif /* GSTCONTROLLER_H_ */
