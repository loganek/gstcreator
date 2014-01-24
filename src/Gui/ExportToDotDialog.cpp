/*
 * gstcreator
 *     ExportToDotDialog.cpp
 *
 *  Created on: 24 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "ExportToDotDialog.h"
#include "ui_ExportToDotDialog.h"
#include <QCheckBox>
#include <QFileDialog>
#include <cstdlib>

ExportToDotDialog::ExportToDotDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::ExportToDotDialog)
{
	ui->setupUi(this);

	ui->pathLineEdit->setText(getenv("GST_DEBUG_DUMP_DOT_DIR"));
}

ExportToDotDialog::~ExportToDotDialog()
{
	delete ui;
}

int ExportToDotDialog::get_graph_details() const
{
	return (ui->statesCheckBox->isChecked() << 0) |
			(ui->statesCheckBox->isChecked() << 1) |
			(ui->statesCheckBox->isChecked() << 2) |
			(ui->statesCheckBox->isChecked() << 3);
}

std::string ExportToDotDialog::get_filename() const
{
	return ui->filenameLineEdit->text().toStdString();
}

bool ExportToDotDialog::is_master_model() const
{
	return ui->mainBinRadioButton->isChecked();
}
