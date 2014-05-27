/*
 * ProbesWatcherDialog.h
 *
 *  Created on: May 23, 2014
 *      Author: loganek
 */

#ifndef PROBESWATCHERDIALOG_H_
#define PROBESWATCHERDIALOG_H_

#include "Logic/IProbeObserver.h"
#include <QStandardItemModel>
#include <QDialog>
#include <functional>

namespace Ui{class ProbesWatcherDialog;}

class ProbesWatcherDialog : public QDialog, public IProbeObserver
{
	Q_OBJECT

public:
	explicit ProbesWatcherDialog(QWidget *parent = 0);
	virtual ~ProbesWatcherDialog();

	void event_probe(const Glib::RefPtr<Gst::Pad>& pad, const Glib::RefPtr<Gst::Event>& event) override;
	void query_probe(const Glib::RefPtr<Gst::Pad>& pad, Glib::RefPtr<Gst::Query> query) override;
	void buffer_probe(const Glib::RefPtr<Gst::Pad>& pad, Glib::RefPtr<Gst::Buffer> buffer) override;

private:
	Ui::ProbesWatcherDialog *ui;
	QStandardItemModel* buffer_model;

	QStandardItem* parse_struct(const Gst::Structure& str);
	static std::function<QString(bool)> bool_to_str;
};

#endif /* PROBESWATCHERDIALOG_H_ */
