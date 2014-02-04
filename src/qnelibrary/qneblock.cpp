/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "qneblock.h"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "qneport.h"

QNEBlock::QNEBlock(const Glib::RefPtr<Gst::Element>& model, QGraphicsItem *parent)
: QGraphicsPathItem(parent),
  model(model)
{
	QPainterPath p;
	p.addRoundedRect(-50, -15, 100, 30, 5, 5);
	setPath(p);
	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	horz_margin = 20;
	vert_margin = 5;
	width = horz_margin;
	height = vert_margin;
}

QNEPort* QNEBlock::add_port(const Glib::RefPtr<Gst::Object>& model, bool isOutput, int flags)
{
	Glib::ustring port_name = model ? model->get_name() : (
				isOutput ? "ai_src" : "ai_sink");
	QNEPort *port = new QNEPort(model, this, port_name.c_str());
	port->set_is_output(isOutput);
	port->set_port_flags(flags);

	QFontMetrics fm(scene()->font());
	int w = fm.width(port_name.c_str());
	int h = fm.height();

	if (w > width - horz_margin)
		width = w + horz_margin;
	height += h;

	QPainterPath p;
	p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);
	setPath(p);

	int y = -height / 2 + vert_margin + port->get_radius();
	for (QGraphicsItem* port_ : childItems())
	{
		if (port_->type() != QNEPort::Type)
			continue;

		QNEPort *port = static_cast<QNEPort*>(port_);
		if (port->is_output())
			port->setPos(width/2 + port->get_radius(), y);
		else
			port->setPos(-width/2 - port->get_radius(), y);
		y += h;
	}

	return port;
}

void QNEBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	if (isSelected())
	{
		painter->setPen(QPen(Qt::darkYellow));
		painter->setBrush(Qt::yellow);
	}
	else
	{
		painter->setPen(QPen(Qt::darkGreen));
		painter->setBrush(Qt::green);
	}

	painter->drawPath(path());
}

QVector<QNEPort*> QNEBlock::get_ports()
{
	QVector<QNEPort*> res;
	for (QGraphicsItem *port_ : childItems())
	{
		if (port_->type() == QNEPort::Type)
			res.append(static_cast<QNEPort*>(port_));
	}
	return res;
}

QVariant QNEBlock::itemChange(GraphicsItemChange change, const QVariant& value)
{
	return value;
}

QNEPort* QNEBlock::find_port(const Glib::RefPtr<Gst::Object>& model)
{
	auto items = childItems();
	for (auto item : items)
		if (item && item->type() == QNEPort::Type)
		{
			QNEPort* port = static_cast<QNEPort*>(item);
			if (port->get_object_model() == model)
				return port;
		}

	return nullptr;
}
