/*
 * gstcreator
 *     PluginsInspectorItem.h
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef PLUGINSINSPECTORITEM_H_
#define PLUGINSINSPECTORITEM_H_

#include <vector>
#include <string>

class PluginsInspectorItem
{
private:
	bool fact;
	std::vector<PluginsInspectorItem*> children;
	PluginsInspectorItem* parent_item;
	std::string factory_name;

public:
	explicit PluginsInspectorItem(bool is_factory, const std::string& name, PluginsInspectorItem *parent = 0);
	virtual ~PluginsInspectorItem();

	void append_child(PluginsInspectorItem* child);

	PluginsInspectorItem* child(int row);
	int child_count() const;
	int row() const;
	PluginsInspectorItem* parent();

	virtual std::string get_name() const;
	bool is_factory() const;
};

#endif /* PLUGINSINSPECTORITEM_H_ */
