/*
 * gstcreator
 *     EventFilter.cpp
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "EventFilter.h"
#include "common.h"
#include <QGraphicsSceneEvent>

EventFilter::EventFilter(QObject* parent)
: QObject(parent)
{}

bool EventFilter::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;

	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{
		switch ((int) me->button())
		{
		case Qt::LeftButton:
			break;
		case Qt::RightButton:
			break;
		}
	}
	case QEvent::GraphicsSceneMouseMove:
		break;
	case QEvent::GraphicsSceneDragMove:
	{
		QGraphicsSceneDragDropEvent* ev = static_cast<QGraphicsSceneDragDropEvent*>(e);

		if (ev == nullptr || !check_mime_data(ev->mimeData()))
		{
			e->ignore();
			return false;
		}
		e->accept();
		return true;
	}
	case QEvent::GraphicsSceneMouseRelease:
		break;
	case QEvent::GraphicsSceneDrop:
		break;
	}

	return QObject::eventFilter(o, e);
}

bool EventFilter::check_mime_data(const QMimeData* mime_data)
{
	return mime_data->hasFormat(DRAG_DROP_FORMAT);
}
