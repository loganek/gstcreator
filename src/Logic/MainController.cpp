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

MainController::MainController()
: gst_controller(Gst::Pipeline::create())
{
}

void MainController::update_current_model(const std::string& model_path)
{
	std::vector<std::string> path = StringUtils::split(model_path, ":");

	Glib::RefPtr<Gst::Object> model = GstUtils::find_element(path, gst_controller.get_master_model());

	if (!model)
		throw std::runtime_error("Cannot find specified bin");

	if (!model->is_bin())
		throw std::runtime_error("Type of model must be BIN");

	gst_controller.update_current_model(Glib::RefPtr<Gst::Bin>::cast_static(model));
}

void MainController::call_command(const std::string& cmd_text)
{
	ObjectExpression* obj_exp = parser.parse(cmd_text);
	std::shared_ptr<Command> cmd = CommandFactory(obj_exp,
			gst_controller.get_current_model()).process();

	cmd->run_command();
}
