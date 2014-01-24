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
{}

void GstController::update_current_model(const RefPtr<Element>& model)
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

Glib::RefPtr<Gst::Element> GstController::get_current_model() const
{
	return current_model;
}
