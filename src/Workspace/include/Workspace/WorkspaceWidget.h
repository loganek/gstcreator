/*
 * gstcreator
 *     WorkspaceWidget.h
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef WORKSPACEWIDGET_H_
#define WORKSPACEWIDGET_H_

#include "EventFilter.h"
#include "Logic/IModelObserver.h"
#include "qnelibrary.h"
#include <QWidget>
#include <QGraphicsView>
#include <gstreamermm.h>

class WorkspaceWidget : public QWidget, public IModelObserver
{
	Q_OBJECT

private:
	QGraphicsView* view;
	QGraphicsScene* scene;
	EventFilter* filter;
	Glib::RefPtr<Gst::Bin> current_model;

	void draw_current_model();
	QNEBlock* find_block(const Glib::RefPtr<Gst::Element>& model);
	QNEPort* find_port(const Glib::RefPtr<Gst::Pad>& pad);

public:
	explicit WorkspaceWidget(QWidget* parent = 0);
	virtual ~WorkspaceWidget();
	void set_model(const Glib::RefPtr<Gst::Bin>& model);

	void resizeEvent(QResizeEvent * event);

	// IModelObserver implementation
	void pad_added(const Glib::RefPtr<Gst::Pad>& pad){}
	void pad_removed(const Glib::RefPtr<Gst::Pad>& pad){}
	void pad_linked(const Glib::RefPtr<Gst::Pad>& proxy_pad);
	void pad_unlinked(const Glib::RefPtr<Gst::Pad>& proxy_pad){}
	void element_added(const Glib::RefPtr<Gst::Element>& element);
	void element_removed(const Glib::RefPtr<Gst::Element>& element){}
};

#endif /* WORKSPACEWIDGET_H_ */
