/*
 * gstcreator
 *     MainController.cpp
 *
 *  Created on: 22 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "MainController.h"
#include "Utils/StringUtils.h"
#include "Utils/GstUtils.h"
#include "Commands/CommandFactory.h"
#include "IGui.h"
#include "common.h"

MainController::MainController()
: gst_controller(Gst::Pipeline::create()),
  gui(nullptr)
{
}

void MainController::update_current_model(const std::string& model_path)
{
	std::vector<std::string> path = StringUtils::split(model_path.c_str(), ":");

	Glib::RefPtr<Gst::Object> model = GstUtils::find_element(path, gst_controller.get_master_model());
	std::vector<std::string> vec_path =
			GstUtils::get_path(gst_controller.get_current_model(), gst_controller.get_master_model());
	vec_path.erase(vec_path.begin());
	std::string prev_path = StringUtils::join(vec_path, ":");

	if (!model)
	{
		safe_call<IGui, void, const std::string&>(gui, &IGui::current_model_changed, prev_path);
		throw std::runtime_error("Cannot find specified bin");
	}
	if (!model->is_bin())
	{
		safe_call<IGui, void, const std::string&>(gui, &IGui::current_model_changed, prev_path);
		throw std::runtime_error("Type of model must be BIN");
	}

	try
	{
		gst_controller.update_current_model(Glib::RefPtr<Gst::Bin>::cast_static(model));
		safe_call<IGui, void, const std::string&>(gui, &IGui::current_model_changed, model_path);
	}
	catch (const std::runtime_error& ex)
	{
		safe_call<IGui, void, const std::string&>(gui, &IGui::current_model_changed, prev_path);
		throw ex;
	}
}

void MainController::call_command(const std::string& cmd_text)
{
	ObjectExpression* obj_exp = parser.parse(cmd_text);
	std::shared_ptr<Command> cmd = CommandFactory(obj_exp,
			gst_controller.get_current_model()).process();

	cmd->run_command();
}

void MainController::set_gui(IGui* gui)
{
	this->gui = gui;
}

void MainController::export_bin_to_file(const std::string& filename,
		int graph_details, bool is_master_model)
{
	gst_controller.export_bin_to_file(filename, graph_details, is_master_model);
}

Glib::RefPtr<Gst::Bin> MainController::get_current_model() const
{
	return gst_controller.get_current_model();
}

void MainController::register_model_observer(IModelObserver* observer)
{
	gst_controller.register_model_observer(observer);
}

void MainController::unregister_model_observer(IModelObserver* observer)
{
	gst_controller.unregister_model_observer(observer);
}
