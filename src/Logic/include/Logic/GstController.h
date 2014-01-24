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
	Glib::RefPtr<Gst::Bin> current_model;

public:
	GstController(const Glib::RefPtr<Gst::Pipeline>& master_model);
	virtual ~GstController(){}

	Glib::RefPtr<Gst::Pipeline> get_master_model() const;
	Glib::RefPtr<Gst::Bin> get_current_model() const;

	void update_current_model(const Glib::RefPtr<Gst::Bin>& model);
	void export_bin_to_file(const std::string& filename, int graph_details, bool current_model);
};

#endif /* GSTCONTROLLER_H_ */
