/*
 * GstObjectManagePanel.h
 *
 *  Created on: May 23, 2014
 *      Author: loganek
 */

#ifndef GSTOBJECTMANAGEPANEL_H_
#define GSTOBJECTMANAGEPANEL_H_

#include "Utils/GstUtils.h"
#include "Logic/IModelObserver.h"
#include <gstreamermm.h>
#include <QtWidgets>

namespace Ui {
class GstObjectManagePanel;
}

class GstObjectManagePanel : public QWidget, public IModelObserver
{
	Q_OBJECT

public:
	explicit GstObjectManagePanel(QWidget *parent = 0);
	~GstObjectManagePanel();

private:
	Ui::GstObjectManagePanel *ui;
	Glib::RefPtr<Gst::Object> selected_item;

	std::vector<QRadioButton*> state_buttons;
	bool state_transaction;

	void state_changed(const Glib::RefPtr<Gst::Element>& element, Gst::State state) override;

	void show_object_info(std::string str, const ObjectNodeInfo& inf, QTreeWidgetItem* parent);
	void clear_layout(QLayout* layout);

public Q_SLOTS:
	void selected_item_changed(const Glib::RefPtr<Gst::Object>& o);
};

#endif /* GSTOBJECTMANAGEPANEL_H_ */
