/*
 * PropertyEnum.cpp
 *
 *  Created on: May 9, 2014
 *      Author: loganek
 */

#include "PropertyEnum.h"
#include <QLayout>
#include <QLabel>
#include <QComboBox>

PropertyEnum::PropertyEnum(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element)
: Property(param_spec, element),
  combobox(nullptr)
{}

void PropertyEnum::build_widget()
{
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout);
	widget->layout()->addWidget(new QLabel(param_spec->name));
	combobox = new QComboBox();

	GEnumValue *values;
	guint j = 0;
	values = G_ENUM_CLASS (g_type_class_ref(param_spec->value_type))->values;

	while (values[j].value_name)
	{
		combobox->addItems({values[j].value_nick});
		j++;
	}

	// can't use qt5 style for this signal, because currentIndexChanged exists in a two variants
	QObject::connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(update_position(int)));
	widget->layout()->addWidget(combobox);
}

void PropertyEnum::update_position(int index)
{
	element->property(param_spec->name, index);
	update_widget();
}

void PropertyEnum::update_widget()
{
	gint enum_value;
	GValue g_value = { 0, };
	g_value_init (&g_value, param_spec->value_type);
	g_object_get_property(G_OBJECT (element->gobj()), param_spec->name, &g_value);
	enum_value = g_value_get_enum (&g_value);
	combobox->setCurrentIndex(enum_value);
}



