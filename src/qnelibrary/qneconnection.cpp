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

#include "qneconnection.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

QNEConnection::QNEConnection(QGraphicsItem *parent)
: QGraphicsPathItem(parent),
  port1(nullptr),
  port2(nullptr)
{
	setPen(QPen(Qt::black, 2));
	setBrush(Qt::NoBrush);
	setZValue(-1);
}

QNEConnection::QNEConnection(QNEPort* first, QNEPort* second, QGraphicsItem* parent)
: QNEConnection(parent)
{
	set_port1(first);
	set_port2(second);
	updatePath();
}

QNEConnection::~QNEConnection()
{
	if (port1)
		port1->get_connections().remove(port1->get_connections().indexOf(this));
	if (port2)
		port2->get_connections().remove(port2->get_connections().indexOf(this));
}

void QNEConnection::set_port1(QNEPort* p)
{
	port1 = p;
	port1->get_connections().append(this);
}

void QNEConnection::set_port2(QNEPort* p)
{
	port2 = p;
	port2->get_connections().append(this);
}

void QNEConnection::updatePath(QPointF pos2)
{
	QPainterPath p;
	QPointF pos1 = port1->scenePos();
	pos2 = (port2 != nullptr) ? this->port2->scenePos(): pos2;

	p.moveTo(pos1);

	qreal dx = pos2.x() - pos1.x();
	qreal dy = pos2.y() - pos1.y();

	QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
	QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

	p.cubicTo(ctr1, ctr2, pos2);

	setPath(p);
}

QNEPort* QNEConnection::get_port1() const
{
	return port1;
}

QNEPort* QNEConnection::get_port2() const
{
	return port2;
}

void QNEConnection::connectColor(int status)
{
	Qt::GlobalColor color;

	switch (status)
	{
	case 0:
		color = Qt::red;
		break;
	case 1:
		color = Qt::green;
		break;
	default:
		color = Qt::black;
	}

	setPen(QPen(color, 2));
}
