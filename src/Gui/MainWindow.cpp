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
#include "Commands.h"
#include "Properties/Property.h"
#include "ExportToDotDialog.h"
#include "common.h"
#include <QLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::MainWindow),
  state_transaction(false)
{
	ui->setupUi(this);
	workspace = new WorkspaceWidget(this);
	ui->workspaceFrame->layout()->addWidget(workspace);
	ui->objectInfoTreeWidget->setColumnCount(2);
	ui->objectInfoTreeWidget->setHeaderLabels(QStringList() << "Key" << "Value");

	connect(workspace, &WorkspaceWidget::selected_item_changed, this, &MainWindow::selected_item_changed);

	state_buttons = {ui->voidPendingRadioButton, ui->nullStateRadioButton,
			ui->readyRadioButton, ui->pausedRadioButton, ui->playingRadioButton};

	for (int i = 0; i < 5; i++)
		connect(state_buttons[i], &QPushButton::toggled, [this, i](bool){
		if (!state_transaction && state_buttons[i]->isChecked() && selected_item && selected_item->is_element())
		{
			auto se = Glib::RefPtr<Gst::Element>::cast_static(selected_item);
			StateCommand((StateType)i, se).run_command();
			Gst::State state, pending;
			se->get_state(state, pending, 0);
			state_changed(se, state);
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

	connect(ui->removeObjectButton, &QPushButton::pressed, [this]{
		if (selected_item)
		{
			if (selected_item->is_element())
			{
				RemoveCommand(Glib::RefPtr<Gst::Element>::cast_static(selected_item)).run_command();
			}
			else if (selected_item->is_pad())
			{
				auto pad = Glib::RefPtr<Gst::Pad>::cast_static(selected_item);
				RemoveCommand(pad, pad->get_parent_element()).run_command();
			}
			else if (Glib::RefPtr<Link>::cast_static(selected_item))
			{
				Glib::RefPtr<Gst::Object> model;
				auto link = Glib::RefPtr<Link>::cast_static(selected_item);

				if (!link->get_source() || !link->get_destination())
				{
					return;
				}
				if (link->get_source()->is_pad())
					model = link->get_source();
				else if (link->get_destination()->is_pad())
					model = link->get_destination();

				if (model)
					UnlinkCommand(Glib::RefPtr<Gst::Pad>::cast_static(model)).run_command();
				// else todo wtf????
			}
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
	connect(up_model_button, &QPushButton::pressed, [this]{
		if (controller->get_current_model()->get_parent() && controller->get_current_model()->get_parent()->is_bin())
		{
			change_current_model(Glib::RefPtr<Gst::Bin>::cast_static(controller->get_current_model()->get_parent()));
		}
	});

	reload_plugin_inspector();
}

void MainWindow::clear_layout(QLayout* layout)
{
	QLayoutItem* item;
	while ((item = layout->takeAt(0)) != nullptr)
	{
		delete item->widget();
		delete item;
	}
}

void MainWindow::selected_item_changed(const Glib::RefPtr<Gst::Object>& o)
{
	ui->objectInfoTreeWidget->clear();
	if (o)
	{
		for (auto a : GstUtils::get_object_info(o))
		{
			show_object_info(a.first, a.second, nullptr);
		}
		selected_item = o;

		if (selected_item->is_element())
		{
			auto se = Glib::RefPtr<Gst::Element>::cast_static(selected_item);

			clear_layout(ui->tabWidget->widget(1)->layout());
			ui->tabWidget->widget(1)->layout()->addWidget(Property::build_property_widget(se));
			Gst::State state, pending;
			se->get_state(state, pending, 0);
			state_changed(se, state);

			clear_layout(ui->requestPadsGroupBox->layout());

			if (se->get_factory())
			{
				auto pad_templates = se->get_factory()->get_static_pad_templates();

				for (auto tpl : pad_templates)
					if (tpl.get_presence() == Gst::PAD_REQUEST)
					{
						auto btn = new QPushButton(tpl.get_name_template().c_str());
						QObject::connect(btn, &QPushButton::clicked, [se, tpl](bool){
							AddCommand(se->get_pad_template(tpl.get_name_template()), se);
						});
						ui->requestPadsGroupBox->layout()->addWidget(btn);
					}
			}
		}
	}
}


void MainWindow::change_current_model(const Glib::RefPtr<Gst::Bin>& bin)
{
	try
	{
		controller->update_current_model(bin);
	}
	catch (const std::runtime_error& ex)
	{
		show_error(ex.what());
	}
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

void MainWindow::state_changed(const Glib::RefPtr<Gst::Element>& element, Gst::State state)
{
	state_transaction = true;
	state_buttons[static_cast<int>(state)]->setChecked(true);
	state_transaction = false;
}

void MainWindow::element_removed(const Glib::RefPtr<Gst::Element>& element, const Glib::RefPtr<Gst::Bin>& bin)
{
	if (element == controller->get_current_model())
		change_current_model(bin);
}

void MainWindow::set_controller(std::shared_ptr<MainController> controller)
{
	this->controller = controller;
	safe_call<WorkspaceWidget, void, const Glib::RefPtr<Gst::Bin>&>(workspace, &WorkspaceWidget::set_model, controller->get_current_model());

	try
	{
		safe_call<MainController, void, IModelObserver*>(controller.get(),
				&MainController::register_model_observer, static_cast<IModelObserver*>(workspace));
		safe_call<MainController, void, IModelObserver*>(controller.get(),
				&MainController::register_model_observer, static_cast<IModelObserver*>(this));

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
