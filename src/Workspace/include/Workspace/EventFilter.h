/*
 * gstcreator
 *     EventFilter.h
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef EVENTFILTER_H_
#define EVENTFILTER_H_

#include "qnelibrary.h"
#include <QObject>
#include <QEvent>
#include <QMimeData>
#include <QPointF>
#include <gstreamermm.h>

class WorkspaceWidget;

class EventFilter : public QObject
{
	Q_OBJECT

private:
	Glib::RefPtr<Gst::Element> model;
	QNEConnection* current_connection;
	QPointF previous_pos;
	WorkspaceWidget* workspace;

	static bool check_mime_data(const QMimeData* mime_data);
	bool drop_block(QEvent* e);
	bool mouse_press_handler(QEvent* e);
	QString get_new_name(const QString& name) const;
	QGraphicsScene* get_scene() const;
	QGraphicsItem* item_at_position(const QPointF &pos) const;

public:
	EventFilter(QGraphicsScene* parent, WorkspaceWidget* workspace);
	bool eventFilter(QObject *o, QEvent *e);
	void set_model(const Glib::RefPtr<Gst::Bin>& model);
	QPointF get_previous_pos() const;
};


#endif /* EVENTFILTER_H_ */
