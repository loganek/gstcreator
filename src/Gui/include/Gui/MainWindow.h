/*
 * gstcreator
 *     MainWindow.h
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "PluginsInspector.h"
#include "Logic/IGui.h"
#include "GstObjectManagePanel.h"
#include "Workspace/WorkspaceWidget.h"
#include "Utils/GstUtils.h"
#include <QMainWindow>
#include <QtWidgets>

namespace Ui{class MainWindow;}

class MainWindow : public QMainWindow, public IGui, public IModelObserver
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

	void set_controller(std::shared_ptr<MainController> controller);
	void current_model_changed(const std::string& model_path);
	std::shared_ptr<MainController> get_controller() const;

	static void show_error(const std::string& err);
private:
	Ui::MainWindow *ui;
	PluginsInspectorTreeView plugins_tree_view;
	PluginsInspectorFilter filter;
	QLineEdit* model_lineedit;
	QPushButton* up_model_button;
	WorkspaceWidget* workspace;
	GstObjectManagePanel* gst_object_manager;

	std::shared_ptr<MainController> controller;

	void reload_plugin_inspector();
	void change_current_model(const Glib::RefPtr<Gst::Bin>& bin);

	void element_removed(const Glib::RefPtr<Gst::Element>& element, const Glib::RefPtr<Gst::Bin>& bin);

private Q_SLOTS:
	void on_actionExport_Bin_To_Dot_File_triggered(bool);
};

#endif /* MAINWINDOW_H_ */
