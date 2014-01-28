/*
 * gstcreator
 *     EventFilter.h
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef EVENTFILTER_H_
#define EVENTFILTER_H_

#include <QObject>
#include <QEvent>
#include <QMimeData>
#include <gstreamermm.h>

class EventFilter : public QObject
{
	Q_OBJECT

private:
	Glib::RefPtr<Gst::Element> model;

	static bool check_mime_data(const QMimeData* mime_data);
	bool drop_block(QEvent* e);
	QString get_new_name(const QString& name) const;

public:
	EventFilter(QObject* parent = 0);
	bool eventFilter(QObject *o, QEvent *e);
	void set_model(const Glib::RefPtr<Gst::Bin>& model);
};


#endif /* EVENTFILTER_H_ */
