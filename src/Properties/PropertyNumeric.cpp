/*
 * PropertyNumeric.cpp
 *
 *  Created on: May 9, 2014
 *      Author: loganek
 */

#include "PropertyNumeric.h"
#include "Utils/StringUtils.h"
#include <QLineEdit>
#include <QLayout>
#include <QLabel>

template<typename T>
PropertyNumeric<T>::PropertyNumeric(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element)
: Property(param_spec, element),
  edit(nullptr)
{}

template<typename T>
void PropertyNumeric<T>::build_widget()
{
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout);
	widget->layout()->addWidget(new QLabel(param_spec->name));
	edit = new QLineEdit();

	QObject::connect(edit, &QLineEdit::returnPressed, [this] {
		try
		{
			T new_value = StringUtils::str_to_numeric<T>(edit->text().toStdString());
			element->property(param_spec->name, new_value);
		}
		catch (...) {}
		update_widget();
	});

	widget->layout()->addWidget(edit);
}

template<typename T>
void PropertyNumeric<T>::update_widget()
{
	T value;
	element->get_property(param_spec->name, value);
	edit->setText(std::to_string(value).c_str());
}

template class PropertyNumeric<gint>;
template class PropertyNumeric<guint>;
template class PropertyNumeric<glong>;
template class PropertyNumeric<gulong>;
template class PropertyNumeric<gint64>;
template class PropertyNumeric<guint64>;
template class PropertyNumeric<gfloat>;
template class PropertyNumeric<gdouble>;

