/*
 * gstcreator
 *     PluginsInspectorItem.cpp
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "PluginsInspectorItem.h"
#include <algorithm>

PluginsInspectorItem::PluginsInspectorItem(const std::string& name, PluginsInspectorItem* parent)
: parent_item(parent),
  factory_name(name)
{
}

PluginsInspectorItem::~PluginsInspectorItem()
{
	for (auto item : children)
		delete item;
}

void PluginsInspectorItem::append_child(PluginsInspectorItem* item)
{
	children.push_back(item);
}

PluginsInspectorItem* PluginsInspectorItem::child(int row)
{
	return children.at(row);
}

int PluginsInspectorItem::child_count() const
{
	return children.size();
}

PluginsInspectorItem* PluginsInspectorItem::parent()
{
	return parent_item;
}

int PluginsInspectorItem::row() const
{
	return (parent_item) ?
			std::find(parent_item->children.begin(),
				parent_item->children.end(), this) - parent_item->children.begin() :
				-1;
}

std::string PluginsInspectorItem::get_name() const
{
	static std::string header_name = "Factory Name";

	return factory_name.empty() ? header_name : factory_name;
}
