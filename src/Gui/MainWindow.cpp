/*
 * gstcreator
 *     MainWindow.cpp
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PluginsInspector.h"
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	auto tree = new PluginsInspectorTreeView;
	layout()->addWidget(tree);

	PluginsInspectorFilter* filter = new PluginsInspectorFilter();
	filter->setSourceModel(new PluginsInspectorModel(FillInspectorMethod::BY_KLASS));

	tree->setModel(filter);
}

MainWindow::~MainWindow()
{
	delete ui;
}



