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
#include "Logic/MainController.h"
#include "qnelibrary.h"
#include <QWidget>
#include <QGraphicsView>
#include <gstreamermm.h>

class MainWindow;

class WorkspaceWidget : public QWidget, public IModelObserver
{
	Q_OBJECT

private:
	QGraphicsView* view;
	QGraphicsScene* scene;
	EventFilter* filter;
	Glib::RefPtr<Gst::Bin> current_model;
	MainWindow* parent_window;

	void draw_current_model();
	QNEBlock* find_block(const Glib::RefPtr<Gst::Element>& model);
	QNEPort* find_port(const Glib::RefPtr<Gst::Pad>& pad);

public:
	explicit WorkspaceWidget(MainWindow* parent);
	virtual ~WorkspaceWidget();
	void set_model(const Glib::RefPtr<Gst::Bin>& model);
	std::shared_ptr<MainController> get_controller() const;

	void change_selected_item(const Glib::RefPtr<Gst::Object>& object);

	void resizeEvent(QResizeEvent * event);

	// IModelObserver implementation
	void pad_added(const Glib::RefPtr<Gst::Pad>& pad);
	void pad_removed(const Glib::RefPtr<Gst::Pad>& pad){}
	void pad_linked(const Glib::RefPtr<Gst::Pad>& proxy_pad);
	void pad_unlinked(const Glib::RefPtr<Gst::Pad>& proxy_pad);
	void element_added(const Glib::RefPtr<Gst::Element>& element);
	void element_removed(const Glib::RefPtr<Gst::Element>& element){}

Q_SIGNALS:
	void selected_item_changed(const Glib::RefPtr<Gst::Object>& object);
};

#endif /* WORKSPACEWIDGET_H_ */
