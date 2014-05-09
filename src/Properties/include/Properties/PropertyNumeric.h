/*
 * PropertyNumeric.h
 *
 *  Created on: May 9, 2014
 *      Author: loganek
 */

#ifndef PROPERTYNUMERIC_H_
#define PROPERTYNUMERIC_H_

#include "Property.h"

class QLineEdit;

template<typename T>
class PropertyNumeric : public Property
{
private:
	QLineEdit* edit;

	void build_widget();
	void update_widget();

public:
	PropertyNumeric(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element);

};


#endif /* PROPERTYNUMERIC_H_ */
