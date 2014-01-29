/*
 * gstcreator
 *     WorkspaceWidget.cpp
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "WorkspaceWidget.h"
#include "qnelibrary.h"
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
	current_model = model;
}

void WorkspaceWidget::element_added(const Glib::RefPtr<Gst::Element>& element)
{
	if (element->get_parent() != current_model)
		return;

	QNEBlock *b = new QNEBlock(element, 0);
	scene->addItem(b);
	b->addPort(element, 0, QNEPort::NamePort);

	auto sink_iterator = element->iterate_sink_pads();
	while (sink_iterator.next())
		b->addInputPort(*sink_iterator);

	auto src_iterator = element->iterate_src_pads();
	while (src_iterator.next())
		b->addOutputPort(*src_iterator);

	for (auto tpl : element->get_factory()->get_static_pad_templates())
		if (tpl.get_presence() == Gst::PAD_SOMETIMES || tpl.get_presence() == Gst::PAD_REQUEST)
			b->addPort(element->get_pad_template(tpl.get_name_template()), tpl.get_direction() == Gst::PAD_SRC);
}
