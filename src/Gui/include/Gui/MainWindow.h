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
	Glib::RefPtr<Gst::Object> current_object;

	static void show_error(const std::string& err);
private:
	Ui::MainWindow *ui;
	PluginsInspectorTreeView plugins_tree_view;
	PluginsInspectorFilter filter;
	QLineEdit* model_lineedit;
	QPushButton* up_model_button;
	WorkspaceWidget* workspace;
	Glib::RefPtr<Gst::Object> selected_item;

	std::shared_ptr<MainController> controller;

	std::vector<QRadioButton*> state_buttons;
	bool state_transaction;

	void reload_plugin_inspector();
	void show_object_info(std::string str, const ObjectNodeInfo& inf, QTreeWidgetItem* parent);
	void change_current_model(const Glib::RefPtr<Gst::Bin>& bin);

	void state_changed(const Glib::RefPtr<Gst::Element>& element, Gst::State state);
	void element_removed(const Glib::RefPtr<Gst::Element>& element, const Glib::RefPtr<Gst::Bin>& bin);

	void clear_layout(QLayout* layout);

private Q_SLOTS:
	void selected_item_changed(const Glib::RefPtr<Gst::Object>& o);

private Q_SLOTS:
	void on_actionExport_Bin_To_Dot_File_triggered(bool);
};

#endif /* MAINWINDOW_H_ */
