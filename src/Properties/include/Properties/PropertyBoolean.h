/*
 * PropertyBoolean.h
 *
 *  Created on: Apr 12, 2014
 *      Author: loganek
 */

#ifndef PROPERTYBOOLEAN_H_
#define PROPERTYBOOLEAN_H_

#include "Property.h"

class QCheckBox;

class PropertyBoolean : public Property
{
private:
	QCheckBox* checkbox;

	void build_widget();
	void update_widget();

public:
	PropertyBoolean(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element);
};

#endif /* PROPERTYBOOLEAN_H_ */
