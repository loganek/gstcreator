/*
 * gstcreator
 *     EventFilter.cpp
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "EventFilter.h"
#include "common.h"
#include "Commands.h"
#include "WorkspaceWidget.h"
#include <QDataStream>
#include <QGraphicsSceneEvent>
#include "Gui/MainWindow.h"

using namespace Gst;
using Glib::RefPtr;

EventFilter::EventFilter(QGraphicsScene* parent, WorkspaceWidget* workspace)
: QObject(parent),
  current_connection(nullptr),
  workspace(workspace)
{}

QGraphicsScene* EventFilter::get_scene() const
{
	return static_cast<QGraphicsScene*>(parent());
}

QGraphicsItem* EventFilter::item_at_position(const QPointF &pos) const
{
	QList<QGraphicsItem*> items =
			get_scene()->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	for (auto item : items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return nullptr;
}

void EventFilter::set_model(const RefPtr<Bin>& model)
{
	this->model = model;
}

bool EventFilter::eventFilter(QObject* o, QEvent* e)
{
	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
		if (mouse_press_handler(e))
			return true;
		break;
	case QEvent::GraphicsSceneMouseMove:
	{
		if (current_connection == nullptr)
			break;

		QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent*>(e);

		if (me == nullptr)
			break;

		current_connection->updatePath(me->scenePos());
		QGraphicsItem *item = item_at_position(me->scenePos());
		if (item && item != current_connection->get_port1() && item->type() == QNEPort::Type)
		{
			QNEPort *src_port = (current_connection->get_port1()->is_output()) ? current_connection->get_port1() : (QNEPort*) item;
			QNEPort *sink_port = (!current_connection->get_port1()->is_output()) ? current_connection->get_port1() : (QNEPort*) item;

			current_connection->connection_color(src_port->can_link(sink_port));
		}
		else
			current_connection->connection_color(2);

		return true;
	}
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
	{
		QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent*>(e);

		if (me == nullptr || !current_connection || me->button() != Qt::LeftButton)
			break;

		QGraphicsItem *item = item_at_position(me->scenePos());

		if (item && item->type() == QNEPort::Type)
		{
			QNEPort* src_port = (current_connection->get_port1()->is_output()) ?
					current_connection->get_port1() : (QNEPort*) item;
			QNEPort* sink_port = (!current_connection->get_port1()->is_output()) ?
				current_connection->get_port1() : (QNEPort*) item;

			if (src_port->can_link(sink_port))
			{
				LinkCommand cmd(sink_port->get_object_model(), src_port->get_object_model());
				cmd.run_command();
			}
		}

		delete current_connection;
		current_connection = nullptr;
		return true;
	}
	case QEvent::GraphicsSceneMouseDoubleClick:
	{
		QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent*>(e);

		if (me == nullptr || me->button() != Qt::LeftButton)
			break;

		QGraphicsItem* item = item_at_position(me->scenePos());

		if (item && item->type() == QNEBlock::Type)
		{
			Glib::RefPtr<Gst::Element> e = static_cast<QNEBlock*>(item)->get_model();
			if (e->is_bin())
				workspace->get_controller()->update_current_model(Glib::RefPtr<Gst::Bin>::cast_static(e));
		}
		return true;
	}
	case QEvent::GraphicsSceneDrop:
		return drop_block(e);
}

return QObject::eventFilter(o, e);
}

bool EventFilter::check_mime_data(const QMimeData* mime_data)
{
	return mime_data->hasFormat(DRAG_DROP_FORMAT);
}

bool EventFilter::mouse_press_handler(QEvent* e)
{
	QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent*>(e);

	if (me == nullptr)
		return false;

	switch (me->button())
	{
	case Qt::LeftButton:
	{
		QGraphicsItem *item = item_at_position(me->scenePos());

		if (!item)
		{
			workspace->change_selected_item(workspace->get_controller()->get_current_model());
			break;
		}
		if (item->type() == QNEPort::Type)
		{
			if (current_connection) delete current_connection;
			current_connection = new QNEConnection();
			get_scene()->addItem(current_connection);
			auto port_item = static_cast<QNEPort*>(item);
			current_connection->set_port1(port_item);
			current_connection->updatePath(me->scenePos());
			workspace->change_selected_item(port_item->get_object_model());
			return true;
		}
		else if (item->type() == QNEBlock::Type)
		{
			auto block_item = static_cast<QNEBlock*>(item);
			workspace->change_selected_item(block_item->get_model());
		}
		else if (item->type() == QNEConnection::Type)
		{
			auto connection_item = static_cast<QNEConnection*>(item);
			workspace->change_selected_item(connection_item->get_model());
		}
		else
		{
			workspace->change_selected_item(workspace->get_controller()->get_current_model());
		}

		break;
	}
	case Qt::RightButton:
		break;
	default:
		break;
	}

	return false;
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
	previous_pos = ev->scenePos();

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

QPointF EventFilter::get_previous_pos() const
{
	return previous_pos;
}
