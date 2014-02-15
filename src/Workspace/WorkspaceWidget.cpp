/*
 * gstcreator
 *     WorkspaceWidget.cpp
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "WorkspaceWidget.h"
#include "Utils/GstUtils.h"
#include "Gui/MainWindow.h"
#include <QResizeEvent>

WorkspaceWidget::WorkspaceWidget(MainWindow* parent)
: QWidget(parent),
  parent_window(parent)
{
	setAcceptDrops(true);
	scene = new QGraphicsScene();
	filter = new EventFilter(scene, this);
	scene->installEventFilter(filter);
	view = new QGraphicsView(scene, this);
	view->setRenderHint(QPainter::Antialiasing, true);
	this->installEventFilter(filter);
}

WorkspaceWidget::~WorkspaceWidget()
{
}

std::shared_ptr<MainController> WorkspaceWidget::get_controller() const
{
	return parent_window->get_controller();
}

void WorkspaceWidget::resizeEvent(QResizeEvent * event)
{
	view->resize(event->size());
}

void WorkspaceWidget::set_model(const Glib::RefPtr<Gst::Bin>& model)
{
	filter->set_model(model);
	current_model = model;
	draw_current_model();
}

void WorkspaceWidget::element_added(const Glib::RefPtr<Gst::Element>& element)
{
	if (element->get_parent() != current_model)
		return;

	QNEBlock *b = new QNEBlock(element, 0);
	scene->addItem(b);
	b->add_port(element, 0, QNEPort::NamePort);

	if (!GstUtils::is_src_element(element))
		b->add_port(Glib::RefPtr<Gst::Object>(), false);
	if (!GstUtils::is_sink_element(element))
		b->add_port(Glib::RefPtr<Gst::Object>(), true);

	auto sink_iterator = element->iterate_sink_pads();
	while (sink_iterator.next())
		b->add_port(*sink_iterator, false);

	auto src_iterator = element->iterate_src_pads();
	while (src_iterator.next())
		b->add_port(*src_iterator, true);

	for (auto tpl : element->get_factory()->get_static_pad_templates())
		if (tpl.get_presence() == Gst::PAD_SOMETIMES || tpl.get_presence() == Gst::PAD_REQUEST)
			b->add_port(element->get_pad_template(tpl.get_name_template()), tpl.get_direction() == Gst::PAD_SRC);

	b->setPos(filter->get_previous_pos());
}

void WorkspaceWidget::pad_added(const Glib::RefPtr<Gst::Pad>& pad)
{
	QNEBlock* block = find_block(pad->get_parent_element());

	if (block == nullptr)
		throw std::runtime_error("cannot draw pad in a block");

	block->add_port(pad, pad->get_direction() == Gst::PAD_SRC);
}

void WorkspaceWidget::pad_linked(const Glib::RefPtr<Gst::Pad>& proxy_pad)
{
	if (proxy_pad->get_direction() == Gst::PAD_SINK)
		return;

	QNEPort* src_port = find_port(proxy_pad->get_peer()),
			*sink_port = find_port(proxy_pad);

	if (!src_port || !sink_port)
		throw std::runtime_error("cannot draw connection between pads");

	scene->addItem(new QNEConnection(src_port, sink_port));
}

QNEPort* WorkspaceWidget::find_port(const Glib::RefPtr<Gst::Pad>& pad)
{
	QNEBlock* block = find_block(pad->get_parent_element());
	return (!block) ? nullptr : block->find_port(pad);
}

QNEBlock* WorkspaceWidget::find_block(const Glib::RefPtr<Gst::Element>& model)
{
	for (auto item : scene->items())
	{
		if (!item || item->type() != QNEBlock::Type)
			continue;

		QNEBlock* block = static_cast<QNEBlock*>(item);

		if (block && block->get_model() == model)
			return block;
	}

	return nullptr;
}


void WorkspaceWidget::draw_current_model()
{
	scene->clear();

	auto iterator = current_model->iterate_elements();

	while (iterator.next())
		element_added(*iterator);
}

void WorkspaceWidget::pad_unlinked(const Glib::RefPtr<Gst::Pad>& proxy_pad)
{
	QNEPort* port = find_port(proxy_pad);

	if (!port)
		return;

	for (auto item : scene->items())
	{
		if (!item || item->type() != QNEConnection::Type)
			continue;

		QNEConnection* con = static_cast<QNEConnection*>(item);

		if ((con->get_port2() && con->get_port2()->get_object_model() == port->get_object_model()) ||
				(con->get_port1() && con->get_port1()->get_object_model() == port->get_object_model()))
		{
			delete item;
			break;
		}
	}
}

void WorkspaceWidget::element_removed(const Glib::RefPtr<Gst::Element>& element, const Glib::RefPtr<Gst::Bin>& bin)
{
	QNEBlock* block = find_block(element);

	if (block)
	{
		delete block;
		// todo maybe clear_selected_item method instead?
		change_selected_item(Glib::RefPtr<Gst::Object>());
	}
}

void WorkspaceWidget::pad_removed(const Glib::RefPtr<Gst::Pad>& pad)
{
	QNEPort* port = find_port(pad);
	if (port)
	{
		delete port;
		change_selected_item(Glib::RefPtr<Gst::Object>());
	}
}

void WorkspaceWidget::change_selected_item(const Glib::RefPtr<Gst::Object>& object)
{
	Q_EMIT selected_item_changed(object);
}
