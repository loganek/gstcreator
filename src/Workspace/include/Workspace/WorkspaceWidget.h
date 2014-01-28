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
#include <QWidget>
#include <QGraphicsView>

class WorkspaceWidget : public QWidget
{
	Q_OBJECT

private:
	QGraphicsView* view;
	QGraphicsScene* scene;
	EventFilter* filter;

public:
	explicit WorkspaceWidget(QWidget* parent = 0);
	virtual ~WorkspaceWidget();

	void resizeEvent(QResizeEvent * event);

};

#endif /* WORKSPACEWIDGET_H_ */
