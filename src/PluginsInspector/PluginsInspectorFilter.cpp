/*
 * gstcreator
 *     PluginsInspectorFilter.cpp
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "PluginsInspectorFilter.h"

PluginsInspectorFilter::PluginsInspectorFilter(QObject* parent):
		QSortFilterProxyModel(parent)
{
}

bool PluginsInspectorFilter::filterAcceptsRow(int source_row,
		const QModelIndex& source_parent) const
{
	return should_be_displayed(sourceModel()->index(source_row, 0, source_parent));
}

bool PluginsInspectorFilter::should_be_displayed(const QModelIndex index) const
{
	if (sourceModel()->rowCount(index) > 0)
	{
		for (int i = 0; i < sourceModel()->rowCount(index); i++)
		{
			QModelIndex child_index = sourceModel()->index(i, 0, index);

			if (!child_index.isValid())
				break;
			if (should_be_displayed(child_index))
				return true;
		}
	}

	QModelIndex use_index = sourceModel()->index(index.row(), 0, index.parent());
	QString type = sourceModel()->data(use_index, Qt::DisplayRole).toString();

	if (type.contains(filterRegExp()))
		return true;

	return false;
}
