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
#include <QLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->workspaceFrame->layout()->addWidget(new WorkspaceWidget);
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
	reload_plugin_inspector();
}

void MainWindow::reload_plugin_inspector()
{
	filter.setSourceModel(new PluginsInspectorModel(ui->inspectorByKlassRadioButton->isChecked() ?
			FillInspectorMethod::BY_KLASS : FillInspectorMethod::BY_PLUGIN));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::set_controller(std::shared_ptr<MainController> controller)
{
	this->controller = controller;
}

void MainWindow::show_error(const std::string& err)
{
	QMessageBox messageBox;
	messageBox.critical(0,"Error", err.c_str());
}

void MainWindow::current_model_changed(const std::string& model_path)
{
	model_lineedit->setText(model_path.c_str());
}

void MainWindow::on_actionExport_Bin_To_Dot_File_triggered(bool)
{
	ExportToDotDialog dialog;

	if (!dialog.exec())
		return;

	controller->export_bin_to_file(dialog.get_filename(), dialog.get_graph_details(), dialog.is_master_model());
}
