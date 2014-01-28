/*
 * gstcreator
 *     EventFilter.cpp
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "EventFilter.h"
#include "common.h"
#include "Commands/AddCommand.h"
#include <QDataStream>
#include <QGraphicsSceneEvent>
#include "Gui/MainWindow.h"

using namespace Gst;
using Glib::RefPtr;

EventFilter::EventFilter(QObject* parent)
: QObject(parent)
{}

void EventFilter::set_model(const RefPtr<Bin>& model)
{
	this->model = model;
}

bool EventFilter::eventFilter(QObject* o, QEvent* e)
{
	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{
		QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent*>(e);
		switch ((int) me->button())
		{
		case Qt::LeftButton:
			break;
		case Qt::RightButton:
			break;
		}
		break;
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
		return drop_block(e);
	}

	return QObject::eventFilter(o, e);
}

bool EventFilter::check_mime_data(const QMimeData* mime_data)
{
	return mime_data->hasFormat(DRAG_DROP_FORMAT);
}

bool EventFilter::drop_block(QEvent* e)
{
	QGraphicsSceneDragDropEvent* ev = static_cast<QGraphicsSceneDragDropEvent*>(e);
	if (ev == nullptr || !check_mime_data(ev->mimeData()))
	{
		e->ignore();
		return false;
	}

	QByteArray item_data = ev->mimeData()->data(DRAG_DROP_FORMAT);
	QDataStream data_stream(&item_data, QIODevice::ReadOnly);
	QString name;
	data_stream >> name;

	RefPtr<Element> element = ElementFactory::create_element(name.toUtf8().constData());
	name = get_new_name(element->get_name().c_str());

	if (name.isEmpty())
		return true;

	element->set_name(name.toUtf8().constData());

	AddCommand cmd(element, model);
	try
	{
		cmd.run_command();
	}
	catch (const std::exception& ex)
	{
		MainWindow::show_error(std::string("Cannot add element: ") + ex.what());
	}
	return true;
}

QString EventFilter::get_new_name(const QString& name) const
{
	bool ok;
	QString new_name = QInputDialog::getText(nullptr, "Select Element's Name",
			tr("Element name:"), QLineEdit::Normal, name, &ok);
	if (ok && !new_name.isEmpty())
		return new_name;
	else
		return QString();
}
