/*
 * gstcreator
 *     WorkspaceWidget.cpp
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "WorkspaceWidget.h"
#include <QResizeEvent>

WorkspaceWidget::WorkspaceWidget(QWidget* parent)
: QWidget(parent)
{
	setAcceptDrops(true);
	filter = new EventFilter();
	scene = new QGraphicsScene();
	scene->installEventFilter(filter);
	view = new QGraphicsView(scene, this);
	view->setRenderHint(QPainter::Antialiasing, true);
	this->installEventFilter(filter);
}

WorkspaceWidget::~WorkspaceWidget()
{
}

void WorkspaceWidget::resizeEvent(QResizeEvent * event)
{
	view->resize(event->size());
}

void WorkspaceWidget::set_model(const Glib::RefPtr<Gst::Bin>& model)
{
	filter->set_model(model);
}
