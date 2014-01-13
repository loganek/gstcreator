/*
 * gstcreator
 *     PluginsInspectorModel.cpp
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "PluginsInspectorModel.h"
#include "Utils/StringUtils.h"

using Glib::RefPtr;
using namespace Gst;
using namespace std;

PluginsInspectorModel::PluginsInspectorModel(FillInspectorMethod method, QObject* parent)
: QAbstractItemModel(parent)
{
	root_item = new PluginsInspectorItem(string());

	RefPtr<Registry> registry = Registry::get();

	for (RefPtr<Plugin> plugin : registry->get_plugin_list())
	{
		if (GST_OBJECT_FLAG_IS_SET (plugin->gobj(), GST_PLUGIN_FLAG_BLACKLISTED))
			return;

		if (method == FillInspectorMethod::BY_KLASS)
			fill_inspector_by_klass(plugin);
		else
			fill_inspector_by_plugin(plugin);
	}
}

PluginsInspectorModel::~PluginsInspectorModel()
{
	delete root_item;
}

PluginsInspectorItem* PluginsInspectorModel::find_parent(PluginsInspectorItem* current_item,
		const vector<string>& klasses, size_t klass_it)
{
	for (int i = 0; i < current_item->child_count(); i++)
	{
		if (current_item->child(i)->get_name() == klasses[klass_it])
		{
			return (klass_it == klasses.size() - 1) ?
					current_item->child(i) :
					find_parent(current_item->child(i), klasses, klass_it + 1);
		}
	}

	PluginsInspectorItem* ret_item;

	for (size_t i = klass_it; i < klasses.size(); i++)
	{
		current_item->append_child(ret_item = new PluginsInspectorItem(klasses[i], current_item));
		current_item = ret_item;
	}

	return ret_item;
}

void PluginsInspectorModel::fill_inspector_by_klass(const RefPtr<Plugin>& plugin)
{
	for (auto feature : Registry::get()->get_feature_list(plugin->get_name()))
		if (feature && GST_IS_ELEMENT_FACTORY (feature->gobj()))
		{
			RefPtr<ElementFactory> factory = factory.cast_static(feature);
			vector<Glib::ustring> metadata = factory->get_metadata_keys();

			if (find(metadata.begin(), metadata.end(), "klass") == metadata.end())
				continue;

			vector<string> klasses =
					StringUtils::split(factory->get_metadata("klass").c_str(), "/");

			PluginsInspectorItem* parent = find_parent(root_item, klasses, 0);
			parent->append_child(new PluginsInspectorItem(factory->get_name(), parent));
		}
}

void PluginsInspectorModel::fill_inspector_by_plugin(const RefPtr<Plugin>& plugin)
{
	auto plugin_item = new PluginsInspectorItem(plugin->get_name().c_str(), root_item);

	root_item->append_child(plugin_item);

	for (auto feature : Registry::get()->get_feature_list(plugin->get_name()))
		if (feature && GST_IS_ELEMENT_FACTORY (feature->gobj()))
			plugin_item->append_child(new PluginsInspectorItem(feature->get_name().c_str(), plugin_item));
}

int PluginsInspectorModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant PluginsInspectorModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole)
		return QVariant();

	PluginsInspectorItem *item = static_cast<PluginsInspectorItem*>(index.internalPointer());
	return QVariant(item->get_name().c_str());
}

Qt::ItemFlags PluginsInspectorModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

	return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

QVariant PluginsInspectorModel::headerData(int section,
		Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return QVariant("Factory name");

	return QVariant();
}

QModelIndex PluginsInspectorModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	PluginsInspectorItem* parent_item;

	parent_item = (parent.isValid()) ?
			static_cast<PluginsInspectorItem*>(parent.internalPointer()):
			root_item;

	PluginsInspectorItem *child_item = parent_item->child(row);

	return  (child_item) ? createIndex(row, column, child_item) :
			QModelIndex();
}

QModelIndex PluginsInspectorModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	auto child_item = static_cast<PluginsInspectorItem*>(index.internalPointer());
	PluginsInspectorItem* parent_item = child_item->parent();

	return  (parent_item == root_item) ?
			QModelIndex() :
			createIndex(parent_item->row(), 0, parent_item);
}

int PluginsInspectorModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
		return 0;

	auto parent_item = (parent.isValid()) ?
			static_cast<PluginsInspectorItem*>(parent.internalPointer()) :
			root_item;

	return parent_item->child_count();
}
