/*
 * PropertyBoolean.cpp
 *
 *  Created on: Apr 12, 2014
 *      Author: loganek
 */

#include "PropertyBoolean.h"
#include <QLayout>
#include <QLabel>
#include <QCheckBox>

PropertyBoolean::PropertyBoolean(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element)
: Property(param_spec, element),
  checkbox(nullptr)
{}

void PropertyBoolean::build_widget()
{
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout);
	widget->layout()->addWidget(new QLabel(param_spec->name));
	checkbox = new QCheckBox();
	widget->layout()->addWidget(checkbox);
}

void PropertyBoolean::update_widget()
{
	gboolean value;
	element->get_property(param_spec->name, value);
	checkbox->setChecked(value);
}



