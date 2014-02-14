/*
 * gstcreator
 *     MainWindow.cpp
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Workspace/WorkspaceWidget.h"
#include "ExportToDotDialog.h"
#include "common.h"
#include <QLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	workspace = new WorkspaceWidget(this);
	ui->workspaceFrame->layout()->addWidget(workspace);
	ui->objectInfoTreeWidget->setColumnCount(2);
	ui->objectInfoTreeWidget->setHeaderLabels(QStringList() << "Key" << "Value");

	connect(workspace, &WorkspaceWidget::selected_item_changed, [this](const Glib::RefPtr<Gst::Object>& o){
		if (o)
		{
			ui->objectInfoTreeWidget->clear();
			for (auto a : GstUtils::get_object_info(o))
			{
				show_object_info(a.first, a.second, nullptr);
			}
		}

	});

	plugins_tree_view.setModel(&filter);
	ui->pluginsInspectorFrame->layout()->addWidget(&plugins_tree_view);

	connect(ui->pluginInspectorFilterLineEdit, SIGNAL(textChanged(QString)),
			&filter, SLOT(setFilterFixedString(QString)));

	connect(ui->inspectorByKlassRadioButton, &QRadioButton::toggled, [this](bool) {
		reload_plugin_inspector();
	});
	connect(ui->inspectorByPluginRadioButton, &QRadioButton::toggled, [this](bool) {
		reload_plugin_inspector();
	});

	connect(ui->runCommandPushButton, &QPushButton::pressed, [this](){
		try
		{
			controller->call_command(ui->commandLineEdit->text().toStdString());
		}
		catch (const std::runtime_error& ex)
		{
			show_error(ex.what());
		}

	});

	ui->statusbar->addWidget(new QLabel("Current model: "));
	model_lineedit = new QLineEdit();
	model_lineedit->setFixedWidth(100);
	ui->statusbar->addWidget(model_lineedit);
	QPushButton* btn = new QPushButton("Set");

	ui->statusbar->addWidget(btn);
	connect(btn, &QPushButton::pressed, [this](){
		try
		{
			controller->update_current_model(model_lineedit->text().toStdString());
		}
		catch (const std::runtime_error& ex)
		{
			show_error(ex.what());
		}
	});

	up_model_button = new QPushButton("UP");
	up_model_button->setEnabled(false);
	ui->statusbar->addWidget(up_model_button);
	connect(up_model_button, &QPushButton::pressed, [this](){
		if (controller->get_current_model()->get_parent() && controller->get_current_model()->get_parent()->is_bin())
		{
			try
			{
				controller->update_current_model(Glib::RefPtr<Gst::Bin>::cast_static(controller->get_current_model()->get_parent()));
			}
			catch (const std::runtime_error& ex)
			{
				show_error(ex.what());
			}
		}
	});

	reload_plugin_inspector();
}

void MainWindow::show_object_info(std::string str, const ObjectNodeInfo& inf, QTreeWidgetItem* parent)
{
	QTreeWidgetItem* item = (parent == nullptr) ?
			new QTreeWidgetItem(ui->objectInfoTreeWidget) : new QTreeWidgetItem(parent);
	item->setText(0, str.c_str());
	item->setText(1, inf.get_value().c_str());

	for (auto a : inf.get_map())
		show_object_info(a.first, a.second, item);
}

void MainWindow::reload_plugin_inspector()
{
	filter.setSourceModel(new PluginsInspectorModel(ui->inspectorByKlassRadioButton->isChecked() ?
			FillInspectorMethod::BY_KLASS : FillInspectorMethod::BY_PLUGIN));
}

MainWindow::~MainWindow()
{
	try
	{
		safe_call<MainController, void, IModelObserver*>(controller.get(),
				&MainController::unregister_model_observer, static_cast<IModelObserver*>(workspace));
	}
	catch (...) { /* ok, we cannot do anything else with this exception */ }
	delete ui;
}

std::shared_ptr<MainController> MainWindow::get_controller() const
{
	return this->controller;
}

void MainWindow::set_controller(std::shared_ptr<MainController> controller)
{
	this->controller = controller;
	safe_call<WorkspaceWidget, void, const Glib::RefPtr<Gst::Bin>&>(workspace, &WorkspaceWidget::set_model, controller->get_current_model());

	try
	{
		safe_call<MainController, void, IModelObserver*>(controller.get(),
				&MainController::register_model_observer, static_cast<IModelObserver*>(workspace));
	}
	catch (const std::runtime_error& ex)
	{
		show_error(std::string("Problems with register observer: ") + ex.what());
	}
}

void MainWindow::show_error(const std::string& err)
{
	QMessageBox messageBox;
	messageBox.critical(0,"Error", err.c_str());
}

void MainWindow::current_model_changed(const std::string& model_path)
{
	model_lineedit->setText(model_path.c_str());
	safe_call<WorkspaceWidget, void, const Glib::RefPtr<Gst::Bin>&>(workspace, &WorkspaceWidget::set_model, controller->get_current_model());
	up_model_button->setEnabled(!model_path.empty());
}

void MainWindow::on_actionExport_Bin_To_Dot_File_triggered(bool)
{
	ExportToDotDialog dialog;

	if (!dialog.exec())
		return;

	controller->export_bin_to_file(dialog.get_filename(), dialog.get_graph_details(), dialog.is_master_model());
}
