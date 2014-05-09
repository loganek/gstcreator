/*
 * PropertyCaps.cpp
 *
 *  Created on: May 9, 2014
 *      Author: loganek
 */

#include "PropertyCaps.h"
#include <QLineEdit>
#include <QLayout>
#include <QLabel>

PropertyCaps::PropertyCaps(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element)
: Property(param_spec, element),
  edit(nullptr)
{}

void PropertyCaps::build_widget()
{
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout);
	widget->layout()->addWidget(new QLabel(param_spec->name));
	edit = new QLineEdit();

	QObject::connect(edit, &QLineEdit::returnPressed, [this] {
		std::string value = edit->text().toStdString();
		Glib::RefPtr<Gst::Caps> caps;

		if (value == "ANY")
		{
			caps = Gst::Caps::create_any();
		}
		else
		{
			caps = Gst::Caps::create_from_string(value);
		}
		caps->reference();
		element->property(param_spec->name, caps);

		update_widget();
	});

	widget->layout()->addWidget(edit);
}

void PropertyCaps::update_widget()
{
	Glib::RefPtr<Gst::Caps> caps;
	element->get_property(param_spec->name, caps);

	if (caps)
	{
		caps->reference();
		edit->setText(caps->to_string().c_str());
	}
}

