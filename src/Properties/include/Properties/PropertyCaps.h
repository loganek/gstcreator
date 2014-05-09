/*
 * PropertyCaps.h
 *
 *  Created on: May 9, 2014
 *      Author: loganek
 */

#ifndef PROPERTYCAPS_H_
#define PROPERTYCAPS_H_

#include "Property.h"

class QLineEdit;

class PropertyCaps : public Property
{
private:
	QLineEdit* edit;

	void build_widget();
	void update_widget();

public:
	PropertyCaps(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element);

};

#endif /* PROPERTYCAPS_H_ */
