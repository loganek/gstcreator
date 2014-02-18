/*
 * gstcreator
 *     PropertyString.h
 *
 *  Created on: 17 lut 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef PROPERTYSTRING_H_
#define PROPERTYSTRING_H_

#include "Property.h"
#include <QLineEdit>

class PropertyString : public Property
{
private:
	QLineEdit* edit;

	void build_widget();
	void update_widget();

public:
	PropertyString(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element);

};


#endif /* PROPERTYSTRING_H_ */
