/*
 * PropertyEnum.h
 *
 *  Created on: May 9, 2014
 *      Author: loganek
 */

#ifndef PROPERTYENUM_H_
#define PROPERTYENUM_H_

#include "Property.h"
#include <QObject>

class QComboBox;

class PropertyEnum : public QObject, public Property
{
	Q_OBJECT
private:
	QComboBox* combobox;

	void build_widget() override;
	void update_widget() override;

private Q_SLOTS:
	void update_position(int index);

public:
	PropertyEnum(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element);
};

#endif /* PROPERTYENUM_H_ */
