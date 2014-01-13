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
#include <gstreamermm.h>

enum class FillInspectorMethod
{
	BY_KLASS,
	BY_PLUGIN
};

class PluginsInspectorModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	std::vector<PluginsInspectorItem*> items;
	PluginsInspectorItem* root_item;

	PluginsInspectorItem* find_parent(PluginsInspectorItem* current_item,
			const std::vector<std::string>& klasses, size_t klass_it);

	void fill_inspector_by_plugin(const Glib::RefPtr<Gst::Plugin>& plugin);
	void fill_inspector_by_klass(const Glib::RefPtr<Gst::Plugin>& plugin);

public:
	explicit PluginsInspectorModel(FillInspectorMethod method, QObject* parent = nullptr);
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
