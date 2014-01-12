/*
 * gstcreator
 *     PluginsInspectorModel.h
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef PLUGINSINSPECTORMODEL_H_
#define PLUGINSINSPECTORMODEL_H_

#include "PluginsInspectorItem.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class PluginsInspectorModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	std::vector<PluginsInspectorItem*> items;
	PluginsInspectorItem* root_item;

public:
	explicit PluginsInspectorModel(QObject* parent = nullptr);
	virtual ~PluginsInspectorModel();

	QVariant data(const QModelIndex& index, int role) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
			const QModelIndex& parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& index) const;
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
};

#endif /* PLUGINSINSPECTORMODEL_H_ */
