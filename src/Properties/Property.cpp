/*
 * gstcreator
 *     Property.cpp
 *
 *  Created on: 16 lut 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Properties.h"
#include <QScrollArea>
#include <QLayout>

using namespace Gst;
using Glib::RefPtr;

Property::Property(GParamSpec* param_spec, const RefPtr<Element>& element)
: param_spec(param_spec),
  element(element),
  widget(nullptr)
{

}

Property::~Property()
{
}

void Property::property_changed(const RefPtr<Element>& element, const std::string& name)
{
	if (element != this->element || name != param_spec->name)
		return;

	update_widget();
}

QWidget* Property::build_property_widget(const RefPtr<Element>& element)
{
	guint property_count;
	QWidget* widget = new QWidget();
	widget->setLayout(new QVBoxLayout);
	QScrollArea* scroll_area = new QScrollArea();

	GParamSpec **property_specs = g_object_class_list_properties(
			G_OBJECT_GET_CLASS(element->gobj()), &property_count);

	for (size_t i = 0; i < property_count; i++)
	{
		Property* property = build_property(property_specs[i], element, "");
		if (property != nullptr)
		{
			widget->layout()->addWidget(property->get_widget());
			property->update_widget();
		}
	}

	scroll_area->setWidget(widget);

	return scroll_area;
}

Property* Property::build_property(GParamSpec* param_spec,
		const RefPtr<Element>& element, const std::string& str_value)
{
	GValue value = { 0, };
	g_value_init (&value, param_spec->value_type);
	GType value_type = G_VALUE_TYPE(&value);

	switch (value_type)
	{
	case G_TYPE_STRING:
		return new PropertyString(param_spec, element);
	case G_TYPE_BOOLEAN:
		return new PropertyBoolean(param_spec, element);
	case G_TYPE_INT:
		return new PropertyNumeric<gint>(param_spec, element);
	case G_TYPE_UINT:
		return new PropertyNumeric<guint>(param_spec, element);
	case G_TYPE_LONG:
		return new PropertyNumeric<glong>(param_spec, element);
	case G_TYPE_ULONG:
		return new PropertyNumeric<gulong>(param_spec, element);
	case G_TYPE_INT64:
		return new PropertyNumeric<gint64>(param_spec, element);
	case G_TYPE_UINT64:
		return new PropertyNumeric<guint64>(param_spec, element);
	case G_TYPE_FLOAT:
		return new PropertyNumeric<gfloat>(param_spec, element);
	case G_TYPE_DOUBLE:
		return new PropertyNumeric<gdouble>(param_spec, element);
	default:
		break;
	}

	if (G_IS_PARAM_SPEC_ENUM (param_spec))
	{
		return new PropertyEnum(param_spec, element);
	}
	else if (!strcmp("GstCaps", g_type_name(value_type)))
	{
		return new PropertyCaps(param_spec, element);
	}
	return nullptr;
}

QWidget* Property::get_widget()
{
	if (widget == nullptr)
		build_widget();

	return widget;
}
