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

#include "qneport.h"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>

#include "qneconnection.h"
#include "qneblock.h"
#include "Utils/GstUtils.h"

using Glib::RefPtr;
using namespace Gst;

QNEPort::QNEPort(const RefPtr<Object>& model, QNEBlock* parent, const QString& name)
: QGraphicsPathItem(parent),
  block(parent),
  model(model)
{
	label = new QGraphicsTextItem(this);
	label->setPlainText(this->name = name);

	radius = 5;
	margin = 2;

	QPainterPath p;
	p.addEllipse(-radius, -radius, 2*radius, 2*radius);
	setPath(p);

	setPen(QPen(Qt::darkRed));
	setBrush(Qt::gray);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	m_portFlags = 0;
}

QNEPort::~QNEPort()
{
	for (QNEConnection* conn : m_connections)
		delete conn;
}

void QNEPort::set_is_output(bool o)
{
	isOutput_ = o;

	QFontMetrics fm(scene()->font());

	if (isOutput_)
		label->setPos(-radius - margin - label->boundingRect().width(), -label->boundingRect().height()/2);
	else
		label->setPos(radius + margin, -label->boundingRect().height()/2);
}

int QNEPort::get_radius()
{
	return radius;
}

bool QNEPort::is_output()
{
	return isOutput_;
}

QVector<QNEConnection*>& QNEPort::get_connections()
{
	return m_connections;
}

void QNEPort::set_port_flags(int f)
{
	m_portFlags = f;
	QFont font(scene()->font());

	if (m_portFlags & TypePort)
		font.setItalic(true);
	else if (m_portFlags & NamePort)
		font.setBold(true);

	if (m_portFlags & (NamePort | TypePort))
	{
		label->setFont(font);
		setPath(QPainterPath());
	}
}

QNEBlock* QNEPort::get_block() const
{
	return block;
}

bool QNEPort::is_connected(QNEPort *other)
{
	for (QNEConnection* conn : m_connections)
		if (conn->get_port1() == other || conn->get_port2() == other)
			return true;

	return false;
}

QVariant QNEPort::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
		for (QNEConnection* conn : m_connections)
			conn->updatePath();

	return value;
}

bool QNEPort::can_link(QNEPort* sink_port) const
{
	auto sink_model = sink_port->get_object_model();

	if (!model)
	{
		auto parent_element = block->get_model();

		if (sink_model && sink_model->is_pad())
			return !!parent_element->create_compatible_pad(RefPtr<Pad>::cast_static(sink_model), RefPtr<Caps>());
		else if (sink_model && sink_model->is_pad_template())
			return !!parent_element->get_compatible_pad_template(RefPtr<PadTemplate>::cast_static(sink_model));
	}
	if (!sink_model || !model)
		return false;

	if (sink_model->is_pad() && model->is_pad())
	{
		auto src = RefPtr<Pad>::cast_static(model),
				sink = RefPtr<Pad>::cast_static(sink_model);

		return (src->get_direction() == sink->get_direction()) ? false :
				src->can_link(sink);
	}


	return false;
}
