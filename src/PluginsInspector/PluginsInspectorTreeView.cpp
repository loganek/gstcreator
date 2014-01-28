/*
 * gstcreator
 *     PluginsInspectorTreeView.cpp
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "PluginsInspectorTreeView.h"
#include "common.h"
#include <QDrag>
#include <QByteArray>
#include <QMimeData>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>


PluginsInspectorTreeView::PluginsInspectorTreeView(QWidget* parent)
: QTreeView(parent)
{
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);

	connect(this, &PluginsInspectorTreeView::pressed, [this](const QModelIndex& index){
		current_factory_name = index.data(Qt::DisplayRole).toString();
	});
	connect(this, &PluginsInspectorTreeView::doubleClicked, this, &PluginsInspectorTreeView::on_object_inspector_double_clicked);
}

PluginsInspectorTreeView::~PluginsInspectorTreeView()
{
}

void PluginsInspectorTreeView::startDrag(Qt::DropActions supportedActions)
{
	QByteArray itemData;
	QDataStream data_stream(&itemData, QIODevice::WriteOnly);

	data_stream << current_factory_name;

	QMimeData* mime_data = new QMimeData;
	mime_data->setData(DRAG_DROP_FORMAT, itemData);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mime_data);
	drag->setPixmap(QLabel(current_factory_name).grab());

	drag->exec(Qt::MoveAction | Qt::CopyAction);
}

void PluginsInspectorTreeView::on_object_inspector_double_clicked(const QModelIndex &index)
{
	QString factory_name = index.data(Qt::DisplayRole).toString();
	QDialog dialog;
	dialog.setWindowTitle("Inspect " + factory_name);
	QHBoxLayout* lay = new QHBoxLayout();
	dialog.setLayout(lay);
	// todo add widget with factory info to a layout
	dialog.exec();
}
