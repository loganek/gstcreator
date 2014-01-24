/*
 * gstcreator
 *     ExportToDotDialog.h
 *
 *  Created on: 24 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef EXPORTTODOTDIALOG_H_
#define EXPORTTODOTDIALOG_H_

#include <QDialog>
#include <string>

namespace Ui{class ExportToDotDialog;}

class ExportToDotDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ExportToDotDialog(QWidget *parent = 0);
	virtual ~ExportToDotDialog();

	int get_graph_details() const;
	std::string get_filename() const;
	bool is_master_model() const;

private:
	Ui::ExportToDotDialog *ui;

};

#endif
