/*
 * gstcreator
 *     PropertyString.cpp
 *
 *  Created on: 17 lut 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "PropertyString.h"
#include <QLayout>
#include <QLabel>

PropertyString::PropertyString(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element)
: Property(param_spec, element),
  edit(nullptr)
{}

void PropertyString::build_widget()
{
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout);
	widget->layout()->addWidget(new QLabel(param_spec->name));
	edit = new QLineEdit();
	widget->layout()->addWidget(edit);
}

void PropertyString::update_widget()
{
	Glib::ustring text;
	element->get_property(param_spec->name, text);
	edit->setText(text.c_str());
}
