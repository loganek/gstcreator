/*
 * gstcreator
 *     PluginsInspectorFilter.h
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef PLUGINSINSPECTORFILTER_H_
#define PLUGINSINSPECTORFILTER_H_

#include <QSortFilterProxyModel>

class PluginsInspectorFilter : public QSortFilterProxyModel
{
private:
	bool should_be_displayed(const QModelIndex index) const;

public:
	PluginsInspectorFilter(QObject* parent = nullptr);
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
};

#endif /* PLUGINSINSPECTORFILTER_H_ */
