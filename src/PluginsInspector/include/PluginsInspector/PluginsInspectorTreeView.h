/*
 * gstcreator
 *     PluginsInspectorTreeView.h
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef PLUGINSINSPECTORTREEVIEW_H_
#define PLUGINSINSPECTORTREEVIEW_H_

#include <QTreeView>

class PluginsInspectorTreeView : public QTreeView
{
	Q_OBJECT

private Q_SLOTS:
	void on_object_inspector_double_clicked(const QModelIndex &index);

private:
	QString current_factory_name;

protected:
	void startDrag(Qt::DropActions supportedActions);

public:
	PluginsInspectorTreeView(QWidget *parent = 0);
	virtual ~PluginsInspectorTreeView();
};

#endif /* PLUGINSINSPECTORTREEVIEW_H_ */
