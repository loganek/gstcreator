/*
 * gstcreator
 *     MainWindow.cpp
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QLayout>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
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



