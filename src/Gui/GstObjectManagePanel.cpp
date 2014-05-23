/*
 * GstObjectManagePanel.cpp
 *
 *  Created on: May 23, 2014
 *      Author: loganek
 */

#include "GstObjectManagePanel.h"
#include "ui_GstObjectManagePanel.h"
#include "Commands.h"
#include "Properties/Property.h"
#include "GstreamerExtensions/Link.h"

// todo move to another file this hack
#include <type_traits>
#include <sigc++/sigc++.h>
namespace sigc
{
template <typename Functor>
struct functor_trait<Functor, false>
{
	typedef decltype (::sigc::mem_fun (std::declval<Functor&> (),
			&Functor::operator())) _intermediate;

	typedef typename _intermediate::result_type result_type;
	typedef Functor functor_type;
};
}

GstObjectManagePanel::GstObjectManagePanel(QWidget *parent)
: QWidget(parent),
  ui(new Ui::GstObjectManagePanel),
  state_transaction(false)
{
	ui->setupUi(this);

	ui->objectInfoTreeWidget->setColumnCount(2);
	ui->objectInfoTreeWidget->setHeaderLabels(QStringList() << "Key" << "Value");

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



	static std::map<Glib::RefPtr<Gst::Pad>, gulong> probe_ides;
	connect(ui->probeBufferCheckBox, &QCheckBox::toggled, [this](bool c) {
		Glib::RefPtr<Gst::Pad> pad = pad.cast_static(selected_item);
		if (!pad)
			return;
		if (c)
		{
			probe_ides[pad] = pad->add_probe(Gst::PAD_PROBE_TYPE_BUFFER, [](const Glib::RefPtr<Gst::Pad>& pad, const Gst::PadProbeInfo& info){
				qDebug() << info.get_id();
				return Gst::PAD_PROBE_OK;
			});
		}
		else
		{
			pad->remove_probe(probe_ides[pad]);
		}
	});
}

GstObjectManagePanel::~GstObjectManagePanel() {
	delete ui;
}

void GstObjectManagePanel::selected_item_changed(const Glib::RefPtr<Gst::Object>& o)
{
	ui->objectInfoTreeWidget->clear();
	ui->requestPadsGroupBox->hide();
	ui->probesGroupBox->hide();
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
							AddCommand(se->get_pad_template(tpl.get_name_template()), se).run_command();
						});
						ui->requestPadsGroupBox->layout()->addWidget(btn);
					}
			}

			ui->requestPadsGroupBox->show();
		}
		else if (selected_item->is_pad())
		{
			ui->probesGroupBox->show();
		}
	}
}

void GstObjectManagePanel::state_changed(const Glib::RefPtr<Gst::Element>& element, Gst::State state)
{
	state_transaction = true;
	state_buttons[static_cast<int>(state)]->setChecked(true);
	state_transaction = false;
}

void GstObjectManagePanel::show_object_info(std::string str, const ObjectNodeInfo& inf, QTreeWidgetItem* parent)
{
	QTreeWidgetItem* item = (parent == nullptr) ?
			new QTreeWidgetItem(ui->objectInfoTreeWidget) : new QTreeWidgetItem(parent);
	item->setText(0, str.c_str());
	item->setText(1, inf.get_value().c_str());

	for (auto a : inf.get_map())
		show_object_info(a.first, a.second, item);
}

void GstObjectManagePanel::clear_layout(QLayout* layout)
{
	QLayoutItem* item;
	while ((item = layout->takeAt(0)) != nullptr)
	{
		delete item->widget();
		delete item;
	}
}

