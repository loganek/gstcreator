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

	buffer_model = new QStandardItemModel(this);
	ui->bufferProbesTreeView->setModel(buffer_model);
}

ProbesWatcherDialog::~ProbesWatcherDialog()
{
	delete ui;
}

template<typename T>
class StructValue
{
private:
	T value;
	QString out = QString::null;

public:
	StructValue(const Structure& str, const Glib::ustring& name) {}
	QString get() { return out.isNull() ? "cannot read value" : out; }
};

template<>
class StructValue<bool>
{
public:
	StructValue(const Structure& str, const Glib::ustring& name)
	{

	}
};

#define CASE_TYPE(g_type, cpp_type, method) \
case g_type: \
{ \
cpp_type value; \
if (str.get_field(name, value)) \
	out = method; \
else \
	out = unknown; \
	break; \
}

void ProbesWatcherDialog::event_probe(const RefPtr<Pad>& pad, const RefPtr<Event>& event)
{
	Structure str = event->get_structure();
	static const QString unknown = "unknown value";

	QStandardItem* str_item = new QStandardItem(QString("Struct: ") + str.get_name().c_str());
	auto bool_to_str = [](bool v) -> QString { return v ? "true" : "false"; };

	for (int i = 0; i < str.size(); i++)
	{
		Glib::ustring name = str.get_nth_field_name(i);
		GType type = str.get_field_type(name);
		QString out;

		switch (type)
		{
		CASE_TYPE(G_TYPE_BOOLEAN, bool, bool_to_str(value))
		CASE_TYPE(G_TYPE_INT, int, QString::number(value))
		CASE_TYPE(G_TYPE_UINT, guint, QString::number(value))
		CASE_TYPE(G_TYPE_DOUBLE, double, QString::number(value))
		CASE_TYPE(G_TYPE_STRING, std::string, QString::fromStdString(value))
		CASE_TYPE(G_TYPE_UINT64, ClockTime, QString::number(value))
		// todo another types: fraction, ranges, enums etc.
		default:
		{
			if (G_TYPE_DATE == type)
			{
				Glib::Date value; \
				out = (str.get_field(name, value)) ?
						value.format_string("%I:%M%p").c_str() : unknown; // todo another formatting
			}
		}
		}

		str_item->appendRow(new QStandardItem(QString(name.c_str()) + ": " + out));
	}

	QStandardItem* item = new QStandardItem("Event");
	item->appendColumn(QList<QStandardItem*>{
		new QStandardItem(QString("Event type: ") + Enums::get_name(event->get_event_type()).c_str()),
				new QStandardItem("Timestamp: " + QString::number(event->get_timestamp())),
				new QStandardItem("Is upstream?: " + bool_to_str(event->is_upstream())),
				new QStandardItem("Is downstream?: " + bool_to_str(event->is_downstream())),
				new QStandardItem("Is serialized? : " + bool_to_str(event->is_serialized())),
				new QStandardItem("Sequence number: " + QString::number(event->get_seqnum()))
	});
	buffer_model->setItem(buffer_model->rowCount(), item);
}

void ProbesWatcherDialog::query_probe(const RefPtr<Pad>& pad, RefPtr<Query> query)
{

}

void ProbesWatcherDialog::buffer_probe(const RefPtr<Pad>& pad, RefPtr<Buffer> buffer)
{
	QStandardItem* item = new QStandardItem("Buffer");
	item->appendColumn(QList<QStandardItem*>{
		new QStandardItem("Size: " + QString::number(buffer->get_size())),
				new QStandardItem("Pts: " + QString::number(buffer->get_pts())),
				new QStandardItem("Dts: " + QString::number(buffer->get_dts())),
				new QStandardItem("Duration: " + QString::number(buffer->get_dts())),
				new QStandardItem("Offset: " + QString::number(buffer->get_dts()))
	});
	buffer_model->setItem(buffer_model->rowCount(), item);
}
