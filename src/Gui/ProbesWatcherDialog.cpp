/*
 * ProbesWatcherDialog.cpp
 *
 *  Created on: May 23, 2014
 *      Author: loganek
 */

#include "ProbesWatcherDialog.h"
#include "ui_ProbesWatcherDialog.h"
#include <QDebug>

using namespace Gst;
using Glib::RefPtr;

ProbesWatcherDialog::ProbesWatcherDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::ProbesWatcherDialog)
{
	ui->setupUi(this);
}

ProbesWatcherDialog::~ProbesWatcherDialog()
{
	delete ui;
}

void ProbesWatcherDialog::event_probe(const RefPtr<Pad>& pad, const RefPtr<Event>& event)
{

}

void ProbesWatcherDialog::query_probe(const RefPtr<Pad>& pad, RefPtr<Query> query)
{

}

void ProbesWatcherDialog::buffer_probe(const RefPtr<Pad>& pad, RefPtr<Buffer> buffer)
{
	qDebug() << buffer->get_size() << endl;
}
